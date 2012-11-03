<?php

/*
 *  This answers the API call to see who is watching what media items now.
 *  It respects the privacy option to publish watching activity
 */
function _kss_watching_now($limit = 10, $who, $sort, $filter3d, $friends) {
	global $user;
	$msg = '';
	if (!is_int($limit)) {
		$msg = 'Limit must be integer value';
	}
	if (!in_array($who, array('all', 'none', 'friends', 'close'))) {
		$msg = 'Who filter must be none, friends, or close';
	}
	if ($who == 'none') {
		return array();
	}
	if (!in_array($sort, array('affinity', 'rating', 'popular', 'newest'))) {
		$msg = 'Sort must be either affinity, rating, popular, or newest';
	}
	if (!in_array($filter3d, array('3d', '2d', 'both'))) {
		$msg = 'Filter 3D must be either 3d, 2d, or both';
	}
	$d3 = " IN ('Yes', 'No') ";
	if ($filter3d == '3d') {
		$d3 = " = 'Yes' ";
	}
	if ($filter3d == '2d') {
		$d3 = " = 'No' ";
	}
	if (!in_array($friends, array('yes', 'no'))) {
		$msg = 'Friends filter must be either yes or no';
	}
 	if (!empty($msg)) {
		return array('status' => 'error', 'message' => $msg);
	}
	$orderby = ' ORDER BY w.start DESC ';
	if ($sort == 'affinity') {
		$orderby = ' ORDER BY r.weight DESC ';
	}
	if ($sort == 'rating' || $sort == 'popular') {
		// TODO split popular query out to use COUNT of watching events rather than rating
		$orderby = ' ORDER BY v.value DESC ';
	}
	$res = db_query("SELECT DISTINCT w.media_id, w.uid FROM {kendra_mediaitem_watch} w
		LEFT JOIN {field_data_field_mediaitem_identifier} i ON w.media_id = i.field_mediaitem_identifier_value
		LEFT JOIN {field_data_field_mediaitem_3d} d ON i.entity_id = d.entity_id
		LEFT JOIN {votingapi_cache} v ON v.entity_id = i.entity_id
		LEFT JOIN {kendra_match_recommender_out_user_to_item} r ON r.recommended_nid = i.entity_id
		WHERE w.stop IS NULL
    AND w.start >= " . strtotime('-2 hour') . "
    AND (v.function = 'average' OR v.function IS NULL)
		AND (d.field_mediaitem_3d_value {$d3} OR d.field_mediaitem_3d_value IS NULL)
		{$orderby}
		LIMIT {$limit}"
    );
	$res = $res->fetchAll();
	$output = array();
	if (!empty($res)) {
		foreach ($res as $media) {
			// check if user of watch event is friend of active user
			if ($who == 'all'
			  || ($who == 'friends' && _kss_friend_of($user->uid, $media->uid))
			  || ($who == 'close' && _kss_close_to($user->uid, $media->uid))
			  ) {

				// check if user has allowed publishing of watching information for this media item
				if (_kendra_saracen_get_mediaitem_tracking($media->media_id, $media->uid)) {

					$id = $media->media_id;
					// quick find user name without loading whole user object
					$account = db_query("SELECT name FROM {users} WHERE uid = :uid", array(':uid' => $media->uid));
					$account = $account->fetchField();
					// lookup media identifier and load the media item node
					$nid = _kendra_saracen_trial_mod_lookup_id($id);
					$media_node = node_load($nid);

					if (!empty($media_node->nid)) {
						$info = field_get_items('node', $media_node, 'field_mediaitem_info');
						$data = array();
						if (!empty($info[0]['value'])) {
			  			$data = unserialize($info[0]['value']);
						}
						$img = (empty($data['image'])) ? '' : $data['image'];
						$output[$id] = array(
							'identifier' => $id,
							'title' => $media_node->title,
							'user' => $account,
							'uid' => $media->uid,
							'image' => $img,
						);
					}
			  }
			}
		}
	}
	return $output;
}

// return true if uid2 is close affinity to uid1
function _kss_close_to($uid1, $uid2) {
	return TRUE;
}

// Is uid2 a saracen friend of uid1
function _kss_friend_of($uid1, $uid2) {
	static $friends;
	if (empty($friends[$uid1])) {
  	$res = db_query("SELECT data FROM {kendra_saracen_friends} f WHERE f.uid = :uid",
	    array(':uid' => $uid1));
	  $data = $res->fetchField();
	  $friends[$uid1] = unserialize($data);
	}
	return isset($friends[$uid1][$uid2]);
}

//function _user_resource_retrieve3($uid) {
//  return "ok";
//}

function _kss_profiles_retrieve($id) {
	$fbid = db_query("SELECT fbu FROM {fb_user} WHERE uid = :uid",
	array(':uid' => $id))->fetchField();
	$nid = db_query("SELECT entity_id FROM {field_data_field_fb_fbid} WHERE field_fb_fbid_value = :fbid",
	array(':fbid' => $fbid))->fetchField();
	$url = url('node/'. $nid, array('absolute' => TRUE));
	$url = 'http://trial.saracen.kendra.org'. url('node/'. $nid);
	$graph = db_query("SELECT id FROM {sparql_store_fb_s2val} WHERE val = :val",
	array(':val' => $url))->fetchField();
	$result = db_query("SELECT s.val as subject,  i.val as predicate , o.val as object FROM sparql_store_fb_triple t
				  INNER JOIN sparql_store_fb_id2val i ON t.p = i.id
				  INNER JOIN sparql_store_fb_s2val s ON t.s = s.id
				  INNER JOIN sparql_store_fb_o2val o ON t.o = o.id
				  WHERE t.s = :val", array(':val' => $graph));
	$profile = array();
	foreach ($result as $record) {
		$profile[] = $record;
	}
	return $profile; // print_r($profile, TRUE);
}

function _kss_social_publish($domain, $msg) {
	if ($domain == 'facebook.com') {
		$fbid = db_query("SELECT fbu FROM {fb_user} WHERE uid = :uid",
		array(':uid' => $user->uid))->fetchField();
		$access_token = db_query("SELECT session_key FROM {fb_user_app} WHERE fbu = :fbu",
		array(':fbu' => $fbid))->fetchField();
		$fb_feed_url = "https://graph.facebook.com/$fbid/feed";

		$result = drupal_http_request($fb_feed_url, array(
		  'method' => 'POST',
		  'data' => "access_token={$access_token}&message={$msg}",
		));

		//watchdog('kss_social_publish', print_r($result, TRUE));
		return $result->status_message;
	} else {
		return "Unknown domain";
	}
}
function _kss_accounts_connect($domain = 'facebook.com,twitter.com') {
	global $user;
	if (!empty($user->uid)) {
		//module_load_include('inc', 'twitter', 'twitter.pages');
		//$output = render(twitter_user_settings($user->uid));
		$connect_url = url('user/' . $user->uid . '/edit/twitter', array('absolute' => TRUE));
    return array('embed' => "<iframe src=\"{$connect_url}\" width=\"600px\" height=\"400px\" />");
	} else {
		if ($domain == 'facebook.com,twitter.com') {
			$connect_url = url('connect', array('absolute' => TRUE));
			return array(
	      'embed' => "<iframe src=\"{$connect_url}\" width=\"300px\" height=\"400px\" />"
	       // . theme('twitter_signin_button'),
			);
		} else if ($domain = 'facebook.com') {
			$connect_url = url('connect', array('absolute' => TRUE));
			return array(
	      'embed' => "<iframe src=\"{$connect_url}\" width=\"300px\" height=\"400px\" />",
			);
		} else if ($domain = 'twitter.com') {
			return theme('twitter_signin_button');
		}
		return "Kendra Social connect error: domain unknown";
  }
}

function _kss_accounts_connect_page() {
	return "<fb:login-button scope=\"\" v=\"2\">Connect</fb:login-button><br />"
	  . theme('twitter_signin_button');
}

/**
 * Define a callback that displays the registration/connect form
 */
function kendra_social_services_menu() {
	$items = array();
	$items['connect'] = array(
    'title' => 'Connect',
    'page callback' => '_kss_accounts_connect_page',
    'access arguments' => array('access content'),
    'type' => MENU_CALLBACK,
	);
	return $items;
}

/**
 * stub service callbacks
 * @todo replace these
 */
function _kss_activitystreams_create () { return _kss_default_callback(); }
function _kss_activitystreams_delete () { return _kss_default_callback(); }
function _kss_activitystreams_retrieve () { return _kss_default_callback(); }
function _kss_activitystreams_update () { return _kss_default_callback(); }
function _kss_albums_create () { return _kss_default_callback(); }
function _kss_albums_delete () { return _kss_default_callback(); }
function _kss_albums_retrieve () { return _kss_default_callback(); }
function _kss_albums_index () { return _kss_default_callback(); }
function _kss_albums_update () { return _kss_default_callback(); }
function _kss_groups_create () { return _kss_default_callback(); }
function _kss_groups_delete () { return _kss_default_callback(); }
function _kss_groups_retrieve () { return _kss_default_callback(); }
function _kss_groups_update () { return _kss_default_callback(); }
function _kss_mediaitems_create () {
  watchdog('debug', '<pre>' . print_r(func_get_args(), TRUE) . '</pre>');
  return _kss_default_callback();
}
function _kss_mediaitems_delete () { return _kss_default_callback(); }
function _kss_mediaitems_retrieve () { return "mediaitem"; return _kss_default_callback(); }
function _kss_mediaitems_update () { return _kss_default_callback(); }
function _kss_people_create () { return _kss_default_callback(); }
function _kss_people_delete () { return _kss_default_callback(); }
function _kss_people_retrieve () { return _kss_default_callback(); }
function _kss_people_update () { return _kss_default_callback(); }
function _kss_profiles_create () { return _kss_default_callback(); }
function _kss_smartfilters_create () { return _kss_default_callback(); }
function _kss_user_login () { return _kss_default_callback(); }
function _kss_user_register () { return _kss_default_callback(); }
function _kss_user_retrieve () { return _kss_default_callback(); }

/**
 * stub access callbacks
 * @todo replace these
 */
function _kss_accounts_connect_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_activitystreams_create_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_activitystreams_delete_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_activitystreams_retrieve_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_activitystreams_update_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_albums_create_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_albums_delete_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_albums_retrieve_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_albums_update_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_groups_create_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_groups_delete_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_groups_retrieve_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_groups_update_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_mediaitems_create_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_mediaitems_delete_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_mediaitems_retrieve_access ($op, $args) { return TRUE; return _kss_default_access_callback($op, $args); }
function _kss_mediaitems_update_access ($op, $args) { return TRUE; return _kss_default_access_callback($op, $args); }
function _kss_people_create_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_people_delete_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_people_retrieve_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_people_update_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_profiles_create_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_profiles_retrieve_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_smartfilters_create_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_social_publish_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_user_retrieve_access ($op, $args) { return _kss_default_access_callback($op, $args); }

function _kss_user_login_access ($op, $args) { return TRUE; }
function _kss_user_register_access ($op, $args) { return TRUE; }
