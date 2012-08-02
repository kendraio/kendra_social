<?php

function _kss_annotation_retrieve($nid) {
	$annotate = node_load($nid);
	return $annotate;
}

function _kss_annotation_delete($nid) {
	$annotate = node_load($id);
	global $user;
	if ($user->uid == $annotate->uid) {
		$annotate->status = 0;
		node_save($annotate);
		return array('status' => 'Unpublished annotaiton with ID ' . $nid);
	} else {
		return array('status' => 'Error', 'message' => 'Operation not allowed, user is not owner of this annotation.');
	}
}

