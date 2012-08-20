<?php
function _kendra_social_albums_add_item($box, $data) {
  $box = node_load($box);
  if (isset($data['item']) && isset($box->nid)) {
    global $user;
    if (_kendra_social_album_access_check('edit', $box, $user)) {
      $media = _kendra_saracen_trial_mod_lookup_id($data['item']);
      if ($media) {
        $endpoints = array(
          array('entity_type' => 'node', 'entity_id' => $box->nid),
          array('entity_type' => 'node', 'entity_id' => $media),
        );
        $relation = relation_create('album_contains', $endpoints);
        $rid = relation_save($relation);
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

function _kendra_social_albums_index() {
	global $user;
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
      $output[$relation->endpoints['und'][0]['entity_id']] = array(
        'id' => $relation->endpoints['und'][0]['entity_id'], 
        'owner' => FALSE,
      );
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
	
  foreach ($user_albums['node'] as $nid => $album) {
    $output[$nid] = array('id' => $nid, 'owner' => TRUE);
  }
  foreach ($output as $nid => $box) {
    $album_node = _kendra_social_albums_retrieve($nid);
    $output[$nid]['title'] = $album_node->title;
    $output[$nid]['description'] = $album_node->description;
    $output[$nid]['item_count'] = count($album_node->items);
  }

  return $output;
}


function _kendra_social_albums_retrieve_no_data($id, $no_data = FALSE) {
  $node = node_load($id);
  $box = new StdClass();
  $box->id = $node->nid;
  $box->title = $node->title;

  $public = field_get_items('node', $node, 'field_album_public');
  $box->public = (empty($public[0]['value'])) ? FALSE : TRUE;
	$friends = field_get_items('node', $node, 'field_album_friends');
  $box->friends = (empty($friends[0]['value'])) ? FALSE : $friends[0]['value'];

  $box->description = $node->field_album_description[$node->language][0]['value'];
  $box->owner = $node->uid;
  $box->owner_name = $node->name;
  if (!$no_data) {
	  $box->items = array();
	  $box->users = array(array('uid' => $node->uid, 'perm' => array('Owner', 'R', 'W')));
	  $items = relation_query('node', $id)->execute();
	 
	  foreach ($items as $item_id => $rel) {
		  if ($rel->relation_type == 'album_contains') {
  		  $rel = relation_load($item_id);
	  	  $media = node_load($rel->endpoints['und'][1]['entity_id']);
		    $box->items[] = array(
			    'identifier' => $media->field_mediaitem_identifier['und'][0]['value'],
			    'mediaitem' => _kendra_saracen_trial_mod_get_mediaitem($media->field_mediaitem_identifier['und'][0]['value'], FALSE),
			  );
			} elseif ($rel->relation_type == 'album_user') {
  		  $rel = relation_load($item_id);
  			// Don't load user - can cause infinite recursion! user load includes boxes
 				// and boxes include users!
   			// $temp_account = user_load($rel->endpoints[LANGUAGE_NONE][1]['entity_id']);
 				$perm = array();
				foreach ($rel->field_album_perm[LANGUAGE_NONE] as $i => $p) {
					if ($p['value'] == 'view') { $perm[] = 'R'; }
					if ($p['value'] == 'edit') { $perm[] = 'W'; }
				}
				$box->users[] = array(
					'uid' => $rel->endpoints[LANGUAGE_NONE][1]['entity_id'],
					'perm' => $perm,
				);
			}
	  }   
  }
  return $box;
}

function _kendra_social_albums_retrieve($id) {
	return _kendra_social_albums_retrieve_no_data($id, FALSE);
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
	// Delete any existing relations for this box and user
	if ($action == 'update') { 
  	_kendra_social_albums_delete_user_rel($bid, $uid);
		// Create new relation with updated permissions
		$endpoints = array(
		  array('entity_type' => 'node', 'entity_id' => $bid),
		  array('entity_type' => 'user', 'entity_id' => $uid),
		);
		$relation = relation_create('album_user', $endpoints);
  } else {
	  // to add or remove perm then load the existing relation
	  $relation = _kendra_social_albums_fetch_user_rel($bid, $uid);
	  if (empty($relation->rid)) {
		  // relation doesn't exist, so create it
			$endpoints = array(
			  array('entity_type' => 'node', 'entity_id' => $bid),
			  array('entity_type' => 'user', 'entity_id' => $uid),
			);
			$relation = relation_create('album_user', $endpoints);
	  }
  }

  // add permission to user
  if ($action == 'update' || $action == 'add') {
		if (strstr($perm, 'R') !== FALSE) {
		  $relation->field_album_perm['und'][] = array('value' => 'view');
		}
		if (strstr($perm, 'W') !== FALSE) {
		  $relation->field_album_perm['und'][] = array('value' => 'edit');
		}		  
	}
	
	// remove permission from user
	if ($action == 'remove') {
		if (strstr($perm, 'R') !== FALSE) {
			_kendra_social_remove_perm($relation->field_album_perm['und'], 'view');
		}
		if (strstr($perm, 'W') !== FALSE) {
			_kendra_social_remove_perm($relation->field_album_perm['und'], 'edit');
		}
	}
	
	// if some permissions then save the relation, else delete
	if (!empty($relation->field_album_perm['und'])) {
	  $rid = relation_save($relation);
	  return $rid;
	} else {
		if (!empty($relation->rid)) {
			relation_delete($relation->rid);
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

function _kendra_social_albums_update_user($id, $data) {
	global $user;
  $box = node_load($id);
  $updates = array();
  $fail = TRUE;

  foreach (explode(',', $data['user']) as $uid) {
    //$account = user_load($uid);
    // check user and box exist
		// Temp skip account check for performance reasons!
    if (isset($box->nid)) { //&& isset($account->uid)) {
	    $fail = FALSE;
	    if (empty($data['action'])) {
				$data['action'] = 'update';
			}
	    $rid = _kendra_social_add_user_to_box($uid, $box->nid, $data['perm'], $data['action']);
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
