<?php

function _kss_annotation_retrieve($nid) {
	$annotate = node_load($nid);
	if (empty($annotate->nid)) {
		$annotate = taxonomy_term_load($nid);
	}
	return $annotate;
}

function _kss_annotation_delete($nid) {
	$annotate = node_load($nid);
	global $user;
	if (empty($annotate->nid)) {
		$tag = taxonomy_term_load($nid);
		if (isset($tag->vocabulary_machine_name) && $tag->vocabulary_machine_name == 'tags') {
			if (empty($tag->description) || $tag->description == "{$user->uid}") {
  			watchdog('tag', "user {$user->uid} deleted tag {$tag->name} ({$tag->tid})");
	  		taxonomy_term_delete($tag->tid);
		  	return array('status' => 'Deleted tag with ID ' . $nid);
		  }
		}
		return array('status' => 'Error', 'message' => 'Invalid ID: Annotation does not exist');
	}
	if ($user->uid == $annotate->uid) {
		$annotate->status = 0;
		node_save($annotate);
		return array('status' => 'Unpublished annotaiton with ID ' . $nid);
	} else {
		return array('status' => 'Error', 'message' => 'Operation not allowed, user is not owner of this annotation.');
	}
}


