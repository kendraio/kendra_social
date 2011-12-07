<?php

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

function _kss_accounts_connect($domain) {
	if ($domain == 'facebook.com') {
		$connect_url = url('connect', array('absolute' => TRUE));
		return array(
      'embed' => "<iframe src=\"{$connect_url}\" width=\"300px\" height=\"400px\" />",
		);
	}
	return "Kendra Social connect error: domain unknown";
}

function _kendra_social_services_connect() {
	return "<fb:login-button scope=\"\" v=\"2\">Connect</fb:login-button>";
	//$form = drupal_get_form('user_register_form');
	//print drupal_render($form);
}

/**
 * Define a callback that displays the registration/connect form
 */
function kendra_social_services_menu() {
	$items = array();
	$items['connect'] = array(
    'title' => 'Connect',
    'page callback' => '_kendra_social_services_connect',
    'access arguments' => array('access content'),
    'type' => MENU_CALLBACK,
	);
	return $items;
}

/**
 * stub service callbacks
 * @todo replace these
 */
function _kss_activitystreams_create () { return _kss_social_default_callback(); }
function _kss_activitystreams_delete () { return _kss_social_default_callback(); }
function _kss_activitystreams_retrieve () { return _kss_social_default_callback(); }
function _kss_activitystreams_update () { return _kss_social_default_callback(); }
function _kss_albums_create () { return _kss_social_default_callback(); }
function _kss_albums_delete () { return _kss_social_default_callback(); }
function _kss_albums_retrieve () { return _kss_social_default_callback(); }
function _kss_albums_update () { return _kss_social_default_callback(); }
function _kss_groups_create () { return _kss_social_default_callback(); }
function _kss_groups_delete () { return _kss_social_default_callback(); }
function _kss_groups_retrieve () { return _kss_social_default_callback(); }
function _kss_groups_update () { return _kss_social_default_callback(); }
function _kss_mediaitems_create () { return _kss_social_default_callback(); }
function _kss_mediaitems_delete () { return _kss_social_default_callback(); }
function _kss_mediaitems_retrieve () { return _kss_social_default_callback(); }
function _kss_mediaitems_update () { return _kss_social_default_callback(); }
function _kss_people_create () { return _kss_social_default_callback(); }
function _kss_people_delete () { return _kss_social_default_callback(); }
function _kss_people_retrieve () { return _kss_social_default_callback(); }
function _kss_people_update () { return _kss_social_default_callback(); }
function _kss_profiles_create () { return _kss_social_default_callback(); }
function _kss_smartfilters_create () { return _kss_social_default_callback(); }
function _kss_user_login () { return _kss_social_default_callback(); }
function _kss_user_register () { return _kss_social_default_callback(); }
function _kss_user_retrieve () { return _kss_social_default_callback(); }

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
function _kss_mediaitems_retrieve_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_mediaitems_update_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_people_create_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_people_delete_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_people_retrieve_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_people_update_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_profiles_create_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_profiles_retrieve_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_smartfilters_create_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_social_publish_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_user_login_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_user_register_access ($op, $args) { return _kss_default_access_callback($op, $args); }
function _kss_user_retrieve_access ($op, $args) { return _kss_default_access_callback($op, $args); }
