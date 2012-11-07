<?php
function _kendra_social_albums_add_item($box, $data) {
  $box = node_load($box);
  if (isset($data['item']) && isset($box->nid)) {
    global $user;
    if (_kendra_social_album_access_check('edit', $box, $user)) {
      $media = _kendra_saracen_trial_mod_lookup_id($data['item']);
      if ($media) {
      	$query = new EntityFieldQuery();
      	$res_boxes = $query->entityCondition('entity_type', 'relation')->entityCondition('bundle', 'album_contains')->fieldCondition('endpoints', 'entity_id', $media, '=')->execute();
      	if (!empty($res_boxes['relation'])) {
      		return array('status' => 'ok', 'message' => "album already contains media item ({$data['item']})");
      	} 
      	$endpoints = array(
          array('entity_type' => 'node', 'entity_id' => $box->nid),
          array('entity_type' => 'node', 'entity_id' => $media),
        );
        $relation = relation_create('album_contains', $endpoints);
        $rid = relation_save($relation);
				if ($user->uid != $box->uid) {
  			  _kss_notification_add('Share', $media, $user->uid, $box->uid, $box->nid, $rid);
				}
        // Add this event to recommender
        _kendra_saracen_save_recommend($user->uid, $media, 'share', $media, FALSE, NULL, 'implicit');

        return array('status' => 'ok', 'message' => "added media item ({$data['item']}) ({$media}) to box ({$box->nid})");
      } else {
        return array('status' => 'error', 'message' => 'no media item found with that id');
      }
    } else {
      return array('status' => 'error', 'message' => 'you do not have access to edit this box');
    }
  } else {
    return array('status' => 'error', 'message' => 'a box ID and item ID is required');
  }
}


function _kendra_social_albums_update($id, $data) {
  global $user;
  $box = node_load($id);
  if (isset($box->nid) && $box->uid == $user->uid) {
    if (isset($data['title'])) {
      $box->title = $data['title'];
    }
    if (isset($data['description'])) {
      $box->field_album_description['und'][0]['value'] = $data['description'];
    }
    if (isset($data['public'])) {
      $box->field_album_public[LANGUAGE_NONE][0]['value'] = (!empty($data['public'])) ? 1 : 0;
    }
    if (isset($data['friends'])) {
      if ($data['friends'] == 1) {
        $data['friends'] = 'R';
      }
      $box->field_album_friends[LANGUAGE_NONE][0]['value'] = _kendra_social_parse_perm($data['friends']);
    }
    node_save($box);
    return array('status' => 'OK', 'message' => 'box ' . $id . ' has been updated');
  } else {
    return array('status' => 'error', 'message' => 'permission denied, not your box');
  }
}

function _kendra_social_parse_perm($perm, $as_array = FALSE) {
  $perm_string = '';
  $perm_array = array();
  if (strstr($perm, 'R') !== FALSE) {
    $perm_array[] = array('value' => 'view');
    $perm_string .= 'R';
  }
  if (strstr($perm, 'W') !== FALSE) {
    $perm_array[] = array('value' => 'edit');
    $perm_string .= 'W';
  }
  return ($as_array) ? $perm_array : $perm_string;
}


function _kendra_social_albums_create($data) {
  global $user;
  $node  = new StdClass();
  $node->type  = 'album';
  $node->status = 1;
  $node->uid = $user->uid;
  $node->language = 'und';
  if (isset($data['title'])) {
    $node->title = $data['title'];
  } else {
    return array('status' => 'error', 'message' => 'Title is required');
  }
  if (isset($data['description'])) {
    $node->field_album_description['und'][0]['value'] = $data['description'];
  }

  if (isset($data['public'])) {
    $node->field_album_public[LANGUAGE_NONE][0]['value'] = 1;
  }
  if (isset($data['friends'])) {
    $node->field_album_friends[LANGUAGE_NONE][0]['value'] = 1;
  }
  node_object_prepare($node);
  node_save($node);
  return array('status' => 'ok', 'identifier' => $node->nid, 'message' => 'created new album');
}

// Check if user has access to box
function _kendra_social_albums_check_perm($box, $uid) {
	$r = FALSE;
	$w = FALSE;
	$account = user_load($box->uid);
	$friends = in_array($uid, array_keys($account->saracen_friends));
  if (!empty($box->field_album_public[LANGUAGE_NONE][0]['value'])) {
		$r = TRUE;
	}
	if ($friends && strstr($box->field_album_friends[LANGUAGE_NONE][0]['safe_value'], 'R')) {
		$r = TRUE;
	}
	if ($friends && strstr($box->field_album_friends[LANGUAGE_NONE][0]['safe_value'], 'W')) {
		$w = TRUE;
	}
	if (!$r || !$w) {
	  $query = new EntityFieldQuery();
	  $res = $query->entityCondition('entity_type', 'relation')
	    ->entityCondition('bundle', 'album_user')
	    ->fieldCondition('endpoints', 'entity_id', $box->nid, '=')
	    ->execute();
	  if (!empty($res['relation'])) {
	    foreach ($res['relation'] as $rel_id => $rel) {
				$rel = relation_load($rel_id);
				foreach ($rel->field_album_perm[LANGUAGE_NONE] as $perm) {
					if ($rel->endpoints[LANGUAGE_NONE][1]['entity_id'] == $uid) {
   					if ($perm['value'] == 'edit') {
	  					$w = TRUE;
		  			}
			  		if ($perm['value'] == 'view') {
				  		$r = TRUE;
				  	}
				  }
				}
			}
		}
	}
	return array('R' => $r, 'W' => $w);
}

function _kendra_social_albums_index($uid = NULL) {
	global $user;
  if (!empty($uid)) {
    // Friend user ID has been provided, show detailed box list for this user
    $query = new EntityFieldQuery();
    $user_albums = $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'album')
      ->propertyCondition('uid', $uid, '=')
      ->execute();
    $boxes = array();
    if (!empty($user_albums['node'])) {
      foreach ($user_albums['node'] as $aid => $v) {
        $box = node_load($aid);
   			$perm = _kendra_social_albums_check_perm($box, $user->uid);
				if ($perm['R'] || $perm['W']) {
          $boxes[$aid] = array(
						'id' => $box->nid,
            'title' => $box->title,
            'description' => $box->field_album_description[LANGUAGE_NONE][0]['safe_value'],
  		   		'owner' => $box->uid,
				  	'public' => empty($box->field_album_public[LANGUAGE_NONE][0]['value']) ? FALSE : TRUE,
					  'friends' => empty($box->field_album_friends[LANGUAGE_NONE][0]['safe_value']) ? "" : $box->field_album_friends[LANGUAGE_NONE][0]['safe_value'],
						'R' => $perm['R'],
						'W' => $perm['W'],
          );
        }
			}
    }
    return $boxes;
  }
  // No UID provided, show boxes summary list for current user
  return _kendra_social_albums_index_fetch($user);
}
function _kendra_social_albums_index_fetch($account = NULL) {
  if (is_numeric($account)) {
    $uid = $account;
    $account = new stdClass();
    $account->uid = $uid;
  }
  if ($account == NULL) {
    global $user;
    $account = $user;
  }
  $query = new EntityFieldQuery();
  $user_albums = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'album')
    ->propertyCondition('uid', $account->uid, '=')
    ->execute();
  $query2 = new EntityFieldQuery();
  $contributed_albums = $query2->entityCondition('entity_type', 'relation')
    ->entityCondition('bundle', 'album_user')
    ->fieldCondition('endpoints', 'entity_id', $account->uid, '=')
    ->execute();
  $output = array();
  if (!empty($contributed_albums['relation'])) {
    foreach ($contributed_albums['relation'] as $rid => $relation) {
      $relation = relation_load($rid);
      if (!empty($relation->endpoints['und'][0]['entity_id'])) {
        $output[$relation->endpoints['und'][0]['entity_id']] = array(
          'id' => $relation->endpoints['und'][0]['entity_id'],
          'owner' => FALSE,
        );
      }
    }
  }

  // load public albums
  $query3 = new EntityFieldQuery();
  $public_albums = $query3->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'album')
    ->fieldCondition('field_album_public', 'value', 1)
    ->propertyCondition('status', 1)
    ->execute();
  if (!empty($public_albums['node'])) {
    foreach ($public_albums['node'] as $nid => $album) {
      $output[$nid] = array('id' => $nid, 'owner' => FALSE);
    }
  }

  // TODO: load saracen friends from cache to use in next query
  // can't do user_load here as causes infinite recursion

  // load friends albums
  $query4 = new EntityFieldQuery();
  $friends_albums = $query4->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'album')
    ->fieldCondition('field_album_friends', 'value', '%R%', 'LIKE')
  //  ->propertyCondition('uid', $friends_array, 'IN')
    ->propertyCondition('status', 1)
    ->execute();
  if (!empty($friends_albums['node'])) {
    foreach ($friends_albums['node'] as $nid => $album) {
      $output[$nid] = array('id' => $nid, 'owner' => FALSE);
    }
  }

  if (!empty($user_albums['node'])) {
    foreach ($user_albums['node'] as $nid => $album) {
      $output[$nid] = array('id' => $nid, 'owner' => TRUE);
    }
  }
  foreach ($output as $nid => $box) {
    $album_node = _kendra_social_albums_retrieve($nid);
    if (!empty($album_node->id)) {
      $output[$nid]['title'] = $album_node->title;
      $output[$nid]['description'] = !empty($album_node->description) ? $album_node->description : '';
      $output[$nid]['item_count'] = isset($album_node->items) ? count($album_node->items) : 0;
    } else {
      //$output[$nid]['debug'] = $album_node;
    }
  }

  return $output;
}

/* Returns TRUE if the given NID is a valid box */
/* Use for testing if a box exists without having to load entire box object */
function _kendra_social_albums_box_exists($id) {
  $nid = db_query("SELECT nid FROM {node} n WHERE n.nid = :nid AND n.type = 'album'",
    array(':nid' => $id))->fetchField();
  return !empty($nid);
}

function _kendra_social_albums_retrieve_no_data($id, $no_data = FALSE, $include_derivatives = FALSE) {
	global $user;
  $node = node_load($id);
  if (empty($node->nid)) {
    // Box does not exist. TODO: also check permission
    return FALSE;
  }
  $box = new StdClass();
  $box->id = $node->nid;
  $box->uid = $node->uid;
  $box->title = $node->title;

  $public = field_get_items('node', $node, 'field_album_public');
  $box->public = (empty($public[0]['value'])) ? FALSE : TRUE;
  $friends = field_get_items('node', $node, 'field_album_friends');
  $box->friends = (empty($friends[0]['value'])) ? FALSE : $friends[0]['value'];

  $description = field_get_items('node', $node, 'field_album_description');
  if (!empty($description)) {
    $box->description = $description[0]['value'];
  }
  $box->owner = $node->uid;
  $box->owner_name = $node->name;
  if (!$no_data) {
    $box->items = array();
    $box->users = array(array('uid' => $node->uid, 'perm' => array('Owner', 'R', 'W')));
    $box->users_temp = array();
    $items = relation_query('node', $id)->execute();

    foreach ($items as $item_id => $rel) {
      if ($rel->relation_type == 'album_contains') {
        $rel = relation_load($item_id);
        $media = node_load($rel->endpoints['und'][1]['entity_id']);
				if ($user->uid == $media->uid || $media->status == 1) {
          $box->items[] = array(
            'identifier' => $media->field_mediaitem_identifier['und'][0]['value'],
            'mediaitem' => _kendra_saracen_trial_mod_get_mediaitem($media->field_mediaitem_identifier['und'][0]['value'], $include_derivatives),
          );
 				}
      } elseif ($rel->relation_type == 'album_user') {
        $rel = relation_load($item_id);
        // Don't load user - can cause infinite recursion! user load includes boxes
        // and boxes include users!
        // $temp_account = user_load($rel->endpoints[LANGUAGE_NONE][1]['entity_id']);
        $perm = array();
        if (!empty($rel->field_album_perm[LANGUAGE_NONE])) {
          foreach ($rel->field_album_perm[LANGUAGE_NONE] as $i => $p) {
            if ($p['value'] == 'view') { $perm[] = 'R'; }
            if ($p['value'] == 'edit') { $perm[] = 'W'; }
          }
        }
        if (!empty($rel->endpoints[LANGUAGE_NONE][1]['entity_id'])) {
          foreach ($perm as $p) {
            $box->users_temp[$rel->endpoints[LANGUAGE_NONE][1]['entity_id']][] = $p;
          }
        }
      }
    }
    foreach ($box->users_temp as $key => $perm) {
      $box->users[] = array(
        'uid' => $key,
        'perm' => $perm,
      );
    }
    unset($box->users_temp);
  }

  return $box;
}

// TODO: Check User1 is friend of User2
function _kss_saracen_friend($uid1, $uid2) {
  return TRUE;
}

function _kendra_social_albums_retrieve($id, $include_derivatives = FALSE) {
  global $user;
  $box = _kendra_social_albums_retrieve_no_data($id, FALSE, $include_derivatives);
  if ($box->uid == $user->uid) {
    return $box;
  }
  if ($box->public) {
    return $box;
  }
  if (strstr($box->friends, 'R') !== FALSE && _kss_saracen_friend($user->uid, $box->uid)) {
    return $box;
  }
  if (!empty($box->users)) {
    foreach ($box->users as $u) {
      if ($u['uid'] == $user->uid && in_array('R', $u['perm'])) {
        return $box;
      }
    }
  }
  return array('status' => 'error', 'message' => 'permission denied');
}


function _kendra_social_albums_delete($id) {
  $album = node_load($id);
  global $user;
  if ($user->uid == $album->uid) {
    node_delete($id);
    return array('status' => 'deleted box with ID ' . $id);
  }
  return array('status' => 'error', 'message' => 'user can only delete their own box.');
}

function _kendra_social_add_user_to_box($uid, $bid, $perm, $action = 'update') {

  $existing = _kendra_social_albums_fetch_user_rel($bid, $uid);
  if (empty($existing->rid)) {
    // relation doesn't exist, so create it
    $endpoints = array(
      array('entity_type' => 'node', 'entity_id' => $bid),
      array('entity_type' => 'user', 'entity_id' => $uid),
    );
    $existing = relation_create('album_user', $endpoints);
  }
  $old_perms = $existing->field_album_perm[LANGUAGE_NONE];
  //watchdog('dd', '<pre>' . print_r($existing, TRUE) . '</pre>');
  $new_perms = array();
  if (!empty($old_perms)) {
    foreach ($old_perms as $p) {
       $new_perms[$p['value']] = 1;
    }
  }

  if ($action == 'update') {
    $new_perms = array();
  }
  if ($action == 'update' || $action == 'add') {
    if (strstr($perm, 'R') !== FALSE) {
      $new_perms['view'] = 1;
    }
    if (strstr($perm, 'W') !== FALSE) {
      $new_perms['edit'] = 1;
    }
  }
  if ($action == 'remove') {
    if (strstr($perm, 'R') !== FALSE) {
      unset($new_perms['view']);
    }
    if (strstr($perm, 'W') !== FALSE) {
      unset($new_perms['edit']);
    }
  }
  $existing->field_album_perm[LANGUAGE_NONE] = array();
  foreach ($new_perms as $p => $val) {
    $existing->field_album_perm[LANGUAGE_NONE][] = array('value' => $p);
  }

  // if some permissions then save the relation, else delete
  if (!empty($new_perms)) {
    $rid = relation_save($existing);
    return $rid;
  } else {
    if (!empty($existing->rid)) {
      relation_delete($existing->rid);
    }
  }
  return FALSE;

}

function _kendra_social_remove_perm(&$perm_array, $perm) {
  if (!empty($perm_array)) {
    foreach ($perm_array as $key => $value) {
      if ($value['value'] == $perm) {
        unset($perm_array[$key]);
      }
    }
  }
  return $perm_array;
}

function _kendra_social_albums_batch_update_user($id, $data) {
  // return array($id, $data);
  // return explode(',', $data['W']);

  // Remove existing permission relations
  $query = new EntityFieldQuery();
  $res = $query->entityCondition('entity_type', 'relation')
    ->entityCondition('bundle', 'album_user')
    ->fieldCondition('endpoints', 'entity_id', $id, '=')
    ->execute();
  if (!empty($res['relation'])) {
    foreach ($res['relation'] as $rel_id => $rel) {
      relation_delete($rel_id);
    }
  }

  // Create new permissions
  $users = array();
  $output = array();
  if (!empty($data['R'])) {
    foreach (explode(',', $data['R']) as $uid) {
      $users[$uid][] = array('value' => 'view');
      $output[] = $uid . '=' . 'R';
    }
  }
  if (!empty($data['W'])) {
    foreach (explode(',', $data['W']) as $uid) {
      $users[$uid][] = array('value' => 'edit');
      $output[] = $uid . '=' . 'W';
    }
  }
  $output = implode(',', $output);

  // Save the new relations
  foreach ($users as $uid => $perms) {
    $endpoints = array(
      array('entity_type' => 'node', 'entity_id' => $id),
      array('entity_type' => 'user', 'entity_id' => $uid),
    );
    $new_r = relation_create('album_user', $endpoints);
    $new_r->field_album_perm[LANGUAGE_NONE] = $perms;
    relation_save($new_r);
  }
  return array('status' => 'ok', 'message' => "Update box ({$id}) permissions to {$output}");
}

function _kendra_social_albums_update_user($id, $data) {

  if (empty($data['action'])) {
    return _kendra_social_albums_batch_update_user($id, $data);
  }

  global $user;
  $box = node_load($id);
  if ($box->uid != $user->uid) {
    return array('status' => 'error', 'message' => 'this is not your box.');
  }
  $bid = $id;
  $updates = array();
  $fail = TRUE;

  foreach (explode(',', $data['user']) as $uid) {
    //$account = user_load($uid);
    // check user and box exist
    // Temp skip account check for performance reasons! and box check!
    if (isset($box->nid)) { //&& isset($account->uid)) {
      $fail = FALSE;
      if (empty($data['action'])) {
        $data['action'] = 'update';
      }
      $rid = _kendra_social_add_user_to_box($uid, $bid, $data['perm'], $data['action']);
      if ($rid) {
        $updates[] = $rid;
      }
    }
  }
  if (!empty($updates)) {
    return array('status' => 'ok', 'message' =>
      "added user ({$data['user']}) to box ({$id})", 'identifier' => implode(',', $updates));
  }
  if ($fail) {
    return array('status' => 'error', 'message' => 'user and album are required');
  }
  return array('status' => 'ok', 'message' => "deleted user ({$data['user']}) from box ({$id})");
}
