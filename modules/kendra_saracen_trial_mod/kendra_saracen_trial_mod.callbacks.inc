<?php

/**
 * _kendra_saracen_trial_mod_annotate_mediaitem
 * annotate a mediaItem
 * force 'type' attribute to Start Case
 */
function _kendra_saracen_trial_mod_annotate_mediaitem($id, $data) {
  global $user;
  $org = node_load(_kendra_saracen_trial_mod_lookup_id($id));
  if (!isset($org->nid)) {
    return array('status' => 'error', 'message' => 'unknown identifier');
  }
  if (!isset($data['type'])) {
    return array('status' => 'error', 'message' => 'required attribute "type" is missing');
  }
  $data['type'] = ucwords($data['type']);
  if ($data['type'] == 'rate' || $data['type'] == 'rating') {
    return array('status' => 'error', 'message' => 'use the dedicated rating API resource');
  }
  $node = new StdClass();
  $node->type = 'annotation';
  $node->status = 1;
  $node->title = 'User annotation for ' . $org->title;
  $node->uid = $user->uid;
  $node->language = 'und';
  $node->field_annotation_mediaitem['und'][0]['value'] = $id;
  if (isset($data['derivative'])) {
    $node->field_annotation_derivative['und'][0]['value'] = $data['derivative'];
  }
  if (isset($data['start']) && $data['start'] > 0) {
    $node->field_annotation_start['und'][0]['value'] = $data['start'];
  }
  if (isset($data['length']) && $data['length'] > 0) {
    $node->field_annotation_length['und'][0]['value'] = $data['length'];
  }
  if (isset($data['duration']) && ($data['duration'] > 0) && !isset($data['length'])) {
    $node->field_annotation_length['und'][0]['value'] = $data['length'];
  }
  if (isset($data['type'])) {
    $node->field_annotation_type['und'][0]['value'] = $data['type'];
  }
  if (isset($data['text'])) {
    $node->field_annotation_text['und'][0]['value'] = $data['text'];
  }

  node_object_prepare($node);
  node_save($node);

  _kss_notification_add($data['type'], $org->nid, $user->uid, $org->uid, $node->nid);
  return array('status' => 'OK', 'message' => 'created user annotation for ' . $org->title,
    'type' => $data['type'],
  );
}

/**
 * _kendra_saracen_trial_mod_delete_mediaitem
 *
 * @note this method only unpublishes the mediaItem (i.e. sets status=0), doesn't delete content
 * @todo evaluate correctness
 */
function _kendra_saracen_trial_mod_delete_mediaitem($id) {
  $nid = _kendra_saracen_trial_mod_lookup_id($id);
  $node = node_load($nid);
  global $user;
  if (!isset($node->nid)) {
    return array('status' => 'error', 'message' => 'unknown identifier');
  }
  if (node_access('delete', $node, $user)) {
	  node_delete($nid);
    return array('status' => 'ok', 'message' => 'deleted media item ' . $title);
  }
  return array('status' => 'error', 'message' => 'permission denied');
}

/**
 * _kendra_saracen_trial_mod_get_annotations
 */
function _kendra_saracen_trial_mod_get_annotations($id) {
  $query       = new EntityFieldQuery();
  $annotations = $query->entityCondition('entity_type', 'node')->entityCondition('bundle', 'annotation')->fieldCondition('field_annotation_mediaitem', 'value', $id, 'like')->execute();
  $results     = array();
  if (isset($annotations['node'])) {
    foreach ($annotations['node'] as $node) {
      $results[] = node_load($node->nid);
    }
  }
  return $results;
}

/**
 * get a single mediaItem
 */
function _kendra_saracen_trial_mod_get_mediaitem($id, $include_derivatives = FALSE) {
  // lookup node based on identifier
  $result = db_query("SELECT entity_id FROM {field_data_field_mediaitem_identifier}
                        WHERE field_mediaitem_identifier_value=:identifier LIMIT 1", array(':identifier' => $id));

  if ($result->rowCount() == 0) {
    $node = node_load(intval($id));
    if (!empty($node)) {
      $result = array($node);
    }
  }
  foreach ($result as $node) {
    if (!isset($node->nid)) {
      $node = node_load($node->entity_id);
    }
    if (empty($node->language)) {
      $node->language = LANGUAGE_NONE;
    }

    // lookup extra properties from sparql
    $store = sparql_get_store(variable_get('kendra_saracen_mediaitem_store', 'mediaItems'), SPARQL_ENDPOINT);
    $url = url('node/' . $node->nid, array('absolute' => TRUE));

    $result = $store->query("SELECT * WHERE { GRAPH <http://localhost/saracen/saracen/0.2/mediaitems> { <{$url}> ?p ?o . } } ");
    $node->data = array();
    foreach ($result['result']['rows'] as $row) {
      $node->data[str_replace($url . '#', '', $row['p'])] = $row['o'];
    }

    if (!empty($node->field_mediaitem_tags[$node->language])) {
      foreach ($node->field_mediaitem_tags[$node->language] as $key => $term) {
        $term = taxonomy_term_load($term['tid']);
        if (!empty($term->tid)) {
          $node->field_mediaitem_tags[$node->language][$key] = array('tid' => $term->tid, 'name' => $term->name);
        }
      }
    }
    if (isset($node->field_mediaitem_info[$node->language][0]['value'])) {
      $node->field_mediaitem_info[$node->language] = unserialize($node->field_mediaitem_info[$node->language][0]['value']);
    }

    if ($include_derivatives) {
      $devquery = new EntityFieldQuery();
      $derivatives = $devquery->entityCondition('entity_type', 'node')->entityCondition('bundle', 'derivative')->fieldCondition('field_derivative_uuid', 'value', $node->field_mediaitem_identifier['und'][0]['value'], 'like')->execute();
      if (isset($derivatives['node'])) {
        foreach ($derivatives['node'] as $deriv) {
          $deriv               = node_load($deriv->nid);
          $uuid                = $deriv->field_derivative_uuid[$deriv->language][0]['value'];
          $deriv->derivativeID = $uuid . ':' . $deriv->nid;

          _kendra_saracen_trial_mod_filter_mediaitem($deriv);
          $node->field_mediaitem_derivatives['und'][] = $deriv;
        }
      }
    }

    // Lookup ratings
    $votes = fivestar_get_votes('node', $node->nid, 'vote');
    if (isset($votes['average']['value'])) {
      $user_vote = isset($votes['user']['value']) ? $votes['user']['value'] : NULL;
      $node->field_mediaitem_rate = array(
        'average' => $votes['average']['value'],
        'count' => $votes['count']['value'],
        'user' => $user_vote,
      );
    }



    // Lookup annotation details
    $annotationquery = new EntityFieldQuery();
    $annotations = $annotationquery->entityCondition('entity_type', 'node')->entityCondition('bundle', 'annotation')->fieldCondition('field_annotation_mediaitem', 'value', $node->field_mediaitem_identifier['und'][0]['value'], 'like')->execute();
    if (isset($annotations['node'])) {
      foreach ($annotations['node'] as $annot) {
        $annot = node_load($annot->nid);
        if (!empty($node->annotations[$annot->field_annotation_type['und'][0]['value']])) {
          $node->annotations[$annot->field_annotation_type['und'][0]['value']]++;
        }
        else {
          $node->annotations[$annot->field_annotation_type['und'][0]['value']] = 1;
        }
      }
      if (isset($node->annotations['Rate'])) {
        unset($node->annotations['Rate']);
      }
    }
    $media_id = $node->field_mediaitem_identifier[LANGUAGE_NONE][0]['value'];
    $count_query = db_query("SELECT COUNT(watch_id) FROM {kendra_mediaitem_watch} WHERE media_id = :id",
      array(':id' => $media_id));
    $node->views_count = intval($count_query->fetchField());

    //_kendra_saracen_trial_mod_filter_mediaitem($node, $include_derivatives);

    $node->boxes = array();

    $query = new EntityFieldQuery();
    $res_boxes = $query->entityCondition('entity_type', 'relation')->entityCondition('bundle', 'album_contains')->fieldCondition('endpoints', 'entity_id', $node->nid, '=')->execute();
    if (!empty($res_boxes['relation'])) {
      //$node->box_relations = $res_boxes['relation'];
      foreach ($res_boxes['relation'] as $relid => $rel) {
        $rel = relation_load($relid);
        $node->boxes[] = _kendra_social_albums_retrieve_no_data($rel->endpoints[LANGUAGE_NONE][0]['entity_id'], TRUE);
      }
    }
    $node->recommended = array();
    $result = db_query("SELECT recommended_nid FROM {kendra_match_recommender_out_item_to_item} WHERE nid = :nid",
      array(':nid' => $node->nid)
    );
    foreach ($result as $rec) {
      $temp = node_load($rec->recommended_nid);
			if (!empty($temp->nid)) {
        $node->recommended[$rec->recommended_nid] = $temp->field_mediaitem_identifier[LANGUAGE_NONE][0]['value'];
			}
    }

//    $watchers = db_select('kendra_mediaitem_watch', 'w')->fields('w')->condition('media_id', $id)->condition('stop', NULL)->execute();
    $watchers = db_select('kendra_mediaitem_watch', 'w')
      ->fields('w')->condition('media_id', $id)->groupBy('uid')
      ->range(0, 10)->orderBy('start', 'DESC')->execute();
    $node->watchers = array();
    foreach ($watchers as $watching) {
	    if (_kendra_saracen_get_mediaitem_tracking($id, $watching->uid)) {
        $node->watchers[] = array(
          'uid' => $watching->uid,
          'started' => $watching->start,
        );
      }
    }

		$publish = _kendra_saracen_get_mediaitem_tracking($id);
		$node->publish_watching_activity = ($publish) ? 1 : 0;

    // TAGS: Load ownership info into media item tags
    global $user;
    // get tags from node and uids from db
    $tags = field_get_items('node', $node, 'field_mediaitem_tags');
    $tid_list = array();
    $db_tags = db_query('SELECT tid, uid FROM {community_tags} WHERE nid = :nid',
      array(':nid' => $node->nid));
    foreach ($db_tags->fetchAll() as $term) {
        $tid_list[$term->tid] = $term->uid;
    }
    // iterate through tags and add uid info
    if (!empty($tags)) {
      foreach ($tags as $key => $term) {
        $tags[$key]['uid'] = $tid_list[$term['tid']];
        $tags[$key]['owner'] = ($tid_list[$term['tid']] == $user->uid);
      }
    }
    $node->field_mediaitem_tags[LANGUAGE_NONE] = $tags;

    return $node;
  }
  return array('status' => 'error', 'message' => 'unknown identifier ' . var_export($id, TRUE));
  //return 'unknown identifier: ' . var_export($id, TRUE);
}

/*
 * Returns true if uid has allowed publish of watched information for the provided media id
 */
function _kendra_saracen_get_mediaitem_tracking($id, $uid = NULL) {
	if (empty($uid)) {
    global $user;
    $uid = $user->uid;
  }
	$tracking = db_query("SELECT * FROM {kendra_user_tracking} WHERE uid = :uid", array(':uid' => $uid));
	$tracking = $tracking->fetchAll();
	if (!empty($tracking)) {
		foreach ($tracking as $track) {
			$items = unserialize($track->data);
			if (isset($items[$id]) && $items[$id] == 1) {
				return TRUE;
			}
		}
	}
	return FALSE;
}


/**
 * filter out unused properties from a MediaItem node object
 */
function _kendra_saracen_trial_mod_filter_mediaitem(&$node, $include_derivatives = FALSE) {
  $mediaItem_properties = array(
    //"vid",
    //"uid",
    "title",
    //"log",
    "status",
    //"comment",
    //"promote",
    //"sticky",
    //"nid",
    "type",
    "language",
    "created",
    "changed",
    //"tnid",
    //"translate",
    //"revision_timestamp",
    //"revision_uid",
    "field_mediaitem_identifier",
    "field_mediaitem_description",
    "field_mediaitem_tags",
    "field_mediaitem_rate",
    "field_mediaitem_category",
    "field_mediaitem_duration",
    "field_mediaitem_3d",
    "field_mediaitem_info",
    //"rdf_mapping",
    //"cid",
    "last_comment_timestamp",
    "last_comment_name",
    "last_comment_uid",
    "comment_count",
    "name",
    //"picture",
    //"data",
    "views_count",
    "field_derivative_format	",
    "field_derivative_codec",
    "field_derivative_quality",
    "field_derivative_url",
    "field_derivative_uuid",
    "derivativeID",
  );

  if ($include_derivatives) {
    $mediaItem_properties[] = 'field_mediaitem_derivatives';
  }

  foreach ($node as $key => $obj) {
    if (!in_array($key, $mediaItem_properties)) {
      unset($node->$key);
    }
  }
}

/**
 * get all mediaItems
 * @todo filter by user preferences
 */
function _kendra_saracen_trial_mod_list_mediaitem($page = 0, $page_size = 10, $category = NULL) {
  global $user;
  $uid = $user->uid;

  if ($page_size == 0) {
    $page_size = 10;
  }

  $page1 = intval($page) * $page_size;
  $page2 = $page1 + $page_size;

  if (!empty($category)) {
    $vocab = taxonomy_vocabulary_machine_name_load('media_category');
		$t_query = new EntityFieldQuery;
		$t_result = $t_query
		  ->entityCondition('entity_type', 'taxonomy_term')
		  ->propertyCondition('name', trim($category))
		  ->propertyCondition('vid', $vocab->vid)
		  ->execute();

    if (!empty($t_result['taxonomy_term'])) {
      $term = array_shift($t_result['taxonomy_term']);
      $category_tid = $term->tid;
    }
  }

	if (!empty($category_tid)) {
		$result = db_query("SELECT n.nid FROM {node} n
			INNER JOIN {field_data_field_mediaitem_category} t ON t.entity_id = n.nid
			WHERE ((n.type = 'mediaitem' AND n.status = 1) OR n.uid = :uid)
			AND t.field_mediaitem_category_tid = :tid
			ORDER BY n.changed DESC LIMIT {$page1}, {$page2}",
		  array(':uid' => $uid, ':tid' => $category_tid));
	} else {
		// if was given category but got here then category was empty
		if (!empty($category)) {
			return array();
		}
    $result = db_query("SELECT nid FROM {node} WHERE (type = 'mediaitem' AND status = 1) OR uid = :uid ORDER BY changed DESC LIMIT {$page1}, {$page2}",
      array(':uid' => $uid));
	}

  $results = array();
  foreach ($result as $node) {
    $results[] = _kendra_saracen_trial_mod_get_mediaitem($node->nid);
  }
  return $results;
}

/**
 * _kendra_saracen_trial_mod_add_rate_mediaitem
 */
function _kendra_saracen_trial_mod_add_rate_mediaitem($id, $data, $lookup_nid = TRUE, $account = FALSE) {
  if ($lookup_nid) {
    $nid = _kendra_saracen_trial_mod_lookup_id($id);
  }
  else {
    $nid = $id;
  }
  if ($account === FALSE) {
    global $user;
    $account = $user->uid;
  }
  // TODO: add error checking so no null result
  _kendra_saracen_save_recommend($account, $nid, 'rating', $data['value']);
  _kendra_saracen_save_recommend($account, $nid, 'like', $data['value']);

  $org_node = node_load($nid);

  $vote = _fivestar_cast_vote('node', $nid, $data['value'], 'vote', $account);
  _kss_notification_add('Rate', $vote['user']['vote_id'], $account, $org_node->uid, $nid);

  return $vote;
  //return func_get_args();
}

/**
 * _kendra_saracen_trial_mod_rate_mediaitem
 */
function _kendra_saracen_trial_mod_rate_mediaitem($id, $data) {
  return _kendra_saracen_trial_mod_add_rate_mediaitem($id, $data);
}

/**
 * _kendra_saracen_trial_mod_register_mediaitem
 * create or update a mediaItem
 *
 * @TODO: refactor this mess!
 */
function _kendra_saracen_trial_mod_register_mediaitem($data = NULL) {
  global $user;
  $op = 'unknown';

  if (!isset($data['identifier'])) {
    if (!isset($data['title'])) {
      return (array('status' => 'error', 'message' => 'Media item Title or Identifier is required.'));
    }
    $data['identifier'] = uniqid($data['title']);
  }
  // use this to determin if reset of title is actually required
  $reset_title = FALSE;
  if (isset($data['title'])) {
    $reset_title = TRUE;
  }
  if (!isset($data['title'])) {
    $reset_title = FALSE;
    $data['title'] = $data['identifier'];
  }

  // Load the node if it exists, or create a new one
  $result = db_query("SELECT entity_id FROM {field_data_field_mediaitem_identifier}
                        WHERE field_mediaitem_identifier_value=:identifier LIMIT 1", array(':identifier' => $data['identifier']));
  $nid = NULL;
  foreach ($result as $record) {
    if (isset($record->entity_id)) {
      $nid = $record->entity_id;
    }
  }
  if (!empty($nid)) {
    $op = 'updated';
    $node = node_load($nid);
    if (!node_access('update', $node, $user)) {
      return array('status' => 'error', 'message' => 'user does not have permission to update ' . $data['identifier']);
    }
  }
  else {
    $op         = 'registered';
    $node       = new StdClass();
    $node->type = 'mediaitem';
    $node->uid  = $user->uid;
    if (isset($data['userid'])) {
      $node->uid = $user->uid;
    }
    $node->language = 'und';
  }

  $node->field_mediaitem_identifier['und'][0]['value'] = $data['identifier'];

  if (isset($node->status)) {
    $node->status = $data['status'];
  }
  elseif ($op == 'registered') {
    $node->status = 1;
  }
  if (empty($node->title) || $reset_title) {
    $node->title = $data['title'];
  }
  if (isset($data['description'])) {
    $node->field_mediaitem_description['und'][0]['value'] = $data['description'];
  }
  if (isset($data['duration'])) {
    $node->field_mediaitem_duration['und'][0]['value'] = $data['duration'];
  }
  if (isset($data['3d'])) {
    $node->field_mediaitem_3d['und'][0]['value'] = $data['3d'];
  }
  if (isset($data['info'])) {
    if (isset($node->field_mediaitem_info['und'][0]['value'])) {
      $current_info = unserialize($node->field_mediaitem_info['und'][0]['value']);
      $node->field_mediaitem_info['und'][0]['value'] = serialize(array_merge($current_info, $data['info']));
    }
    else {
      $node->field_mediaitem_info['und'][0]['value'] = serialize($data['info']);
    }
  }
  if (isset($data['category'])) {
    $vocab = taxonomy_vocabulary_machine_name_load('media_category');
		$t_query = new EntityFieldQuery;
		$t_result = $t_query
		  ->entityCondition('entity_type', 'taxonomy_term')
		  ->propertyCondition('name', trim($data['category']))
		  ->propertyCondition('vid', $vocab->vid)
		  ->execute();

    if (!empty($t_result['taxonomy_term'])) {
      $term = array_shift($t_result['taxonomy_term']);
      $node->field_mediaitem_category['und'][0]['tid'] = $term->tid;
    }
    else {
      $term       = new StdClass();
      $term->name = $data['category'];
      $term->vid  = $vocab->vid;
      taxonomy_term_save($term);
      $node->field_mediaitem_category['und'][0]['tid'] = $term->tid;
    }
  }
  node_object_prepare($node);
  node_save($node);

  /**
   * set keywords
   */
  if (isset($data['keywords'])) {
    $tags = split(',', $data['keywords']);
    foreach ($tags as $tag) {
      $tag = trim($tag);
      _kendra_saracen_trial_mod_tag_mediaitem($data['identifier'], array('tag' => $tag));
    }
  }

  // save data items to triple store
  $store = sparql_get_store(variable_get('kendra_saracen_mediaitem_store', 'mediaItems'), SPARQL_ENDPOINT);
  foreach ($data as $key => $value) {
    $value     = str_replace('"', '\"', trim($value));
    $url       = url('node/' . $node->nid, array('absolute' => TRUE));
    $predicate = url('node/' . $node->nid, array('fragment' => $key, 'absolute' => TRUE));
    $store->query("INSERT INTO <mediaitems> {\n" .
      "  <{$url}> <{$predicate}> \"{$value}\" .\n" . "}\n"
    );
  }

  /*
  * set codec & quality
  */

  if (isset($data['codec']) && isset($data['quality'])) {
    $query = new EntityFieldQuery();
    $derivative = $query->entityCondition('entity_type', 'node')->entityCondition('bundle', 'derivative')->fieldCondition('field_derivative_quality', 'value', $data['quality'], 'like')->fieldCondition('field_derivative_codec', 'value', $data['codec'], 'like')->fieldCondition('field_derivative_uuid', 'value', $data['identifier'], 'like')->execute();

    if (isset($derivative['node']) && count($derivative['node'] > 0)) {
      $derivative_nid = array_keys($derivative['node']);
      $derivative = node_load($derivative_nid[0]);
    }
    else {
      $derivative = new StdClass();
      $derivative->type = 'derivative';
      $derivative->status = 1;
      $derivative->uid = isset($data['userid']) ? $data['userid'] : 1;
      $derivative->title = $data['title'] . ':' . $data['codec'] . ':' . $data['quality'];
      $derivative->language = 'und';
    }
    $derivative->field_derivative_codec['und'][0]['value'] = $data['codec'];
    $derivative->field_derivative_uuid['und'][0]['value'] = $data['identifier'];
    $derivative->field_derivative_quality['und'][0]['value'] = $data['quality'];

    if (isset($data['link'])) {
      $derivative->field_derivative_url['und'][0]['value'] = $data['link'];
    }

    node_object_prepare($derivative);
    node_save($derivative);
    $derivative_id = $derivative->nid;
  }

  if (isset($derivative_id)) {
    $endpoints = array(
      array('entity_type' => 'node', 'entity_id' => $node->nid),
      array('entity_type' => 'node', 'entity_id' => $derivative->nid),
    );
    $relation = relation_create('derivative', $endpoints);
    $rid = relation_save($relation);
  }

  if ($op == 'registered') {
    _kss_notification_add('Import', $node->nid, 0, $node->uid);
  }

  // return notification of registered node
  return array('status' => 'OK', 'message' => 'Media item ' . $op, 'identifier' => $data['identifier']);
}

/**
 * _kendra_saracen_trial_mod_tag_mediaitem
 */
function _kendra_saracen_trial_mod_tag_mediaitem($id, $data) {
  global $user;
  if (empty($data['tag'])) {
	  return array('status' => 'error', 'message' => 'empty tag');
  }
  $node = node_load(_kendra_saracen_trial_mod_lookup_id($id));
  $vid  = array_shift(community_tags_vids_for_node($node));
  $tags = community_tags_flatten(community_tags_get_user_node_tags($user->uid, $node->nid, $vid));
  $tags = array_merge($tags, array($data['tag']));
  community_tags_taxonomy_node_save($node, array('tags' => array($vid => $tags)), FALSE, $user->uid);
  $tags = community_tags_flatten(community_tags_get_user_node_tags($user->uid, $node->nid, $vid));
  _kendra_saracen_save_recommend($user->uid, $node->nid, 'tag', $data['tag']);

  // TODO "Friends tagged" - screen 47
  //_kss_notification_add('Tag', $source, $user_from, $user_to, $source2 = 0);

  return $tags;
}

/**
 * _kendra_saracen_trial_mod_update_mediaitem
 */
function _kendra_saracen_trial_mod_update_mediaitem($id, $data) {
  $data['identifier'] = $id;
  return _kendra_saracen_trial_mod_register_mediaitem($data);
}


function _kendra_saracen_trial_mod_activity_track_mediaitem($id, $publish) {
	if (!in_array($publish, array(0, 1))) {
		return array('status' => 'error', 'message' => 'publish value must be 0 or 1');
	}
	global $user;
	$tracking = db_query("SELECT * FROM {kendra_user_tracking} WHERE uid = :uid", array(':uid' => $user->uid));
	$tracking = $tracking->fetchAll();
	if (!empty($tracking)) {
		foreach ($tracking as $track) {
			$items = unserialize($track->data);
			$items[$id] = $publish;
			db_query("UPDATE {kendra_user_tracking} SET data = :data WHERE uid = :uid", array(':uid' => $user->uid,
			  ':data' => serialize($items)));
		}
	} else {
		db_query("INSERT INTO {kendra_user_tracking} VALUES (:uid, :data)", array(':uid' => $user->uid,
			':data' => serialize(array($id => $publish))));
	}
	return array('status' => 'ok', 'message' => 'set privacy=' . $publish . ' for watching activity for item ' . $id,
	  'identifier' => $id);
}


function _kendra_saracen_trial_mod_user_watched($uid) {
	$res = db_query("SELECT DISTINCT media_id FROM {kendra_mediaitem_watch} WHERE uid = :uid", array(':uid' => $uid));
	$res = $res->fetchAll();
	$output = array();
	foreach ($res as $media) {
		if (_kendra_saracen_get_mediaitem_tracking($media->media_id, $uid)) {
  		$output[] = $media->media_id;
		}
	}
	return $output;

}





