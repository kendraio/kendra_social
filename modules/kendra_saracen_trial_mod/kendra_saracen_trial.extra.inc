<?php

function _kss_annotation_retrieve($nid) {
	$annotate = node_load($nid);
	if (empty($annotate->nid)) {
		$annotate = taxonomy_term_load($nid);
	}
	return $annotate;
}

function _kss_annotation_delete($tid, $identifier) {
  global $user;

  // If data['identifier'] is set then we are deleting a tag.
  // if identifier is not set then we are deleting an annotation node
  if (!empty($identifier)) {
    $nid = _kendra_saracen_trial_mod_lookup_id($identifier);

    $media = node_load($nid);
    $tags = field_get_items('node', $media, 'field_mediaitem_tags');
    if (!empty($tags)) {
	    foreach ($tags as $key => $value) {
		    if ($value['tid'] == $tid) {
			    unset($tags[$key]);
		    }
	    }
    }
    $media->field_mediaitem_tags[LANGUAGE_NONE] = $tags;
    node_save($media);
    if ($user->uid == $media->uid) {
      return array('status' => 'ok', 'message' => "deleted tag {$tid} from {$identifier}");
    } else {
	    return array('status' => 'error', 'message' => 'permission denied, mediaitem');
    }
  }

  // else look for annotation node and unpublish:
	$annotate = node_load($tid);
	if (!empty($annotate->nid)) {
		if ($user->uid == $annotate->uid) {
			// unpublish the annotation. TODO: Should it be deleted?
      node_delete($annotate->nid);
			return array('status' => 'ok', 'message' => "deleted annotation {$annotate->nid}");
		} else {
      $media_id = $annotate->field_annotation_mediaitem[LANGUAGE_NONE][0]['value'];
      $mediaitem_nid = _kendra_saracen_trial_mod_lookup_id($media_id);
      $media_node = node_load($mediaitem_nid);
      if ($user->uid == $media_node->uid) {
        node_delete($annotate->nid);
        return array('status' => 'ok', 'message' => "deleted annotation {$annotate->nid}");
      }
			return array('status' => 'error', 'message' => 'permission denied, annotation');
		}
	}
	return array('status' => 'error', 'message' => 'Invalid ID: Annotation does not exist or mediaitem identifier missing');
}











