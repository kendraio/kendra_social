<?php
/**
 * @file
 * Code for the Kendra Social Services feature.
 */

/**
 * define the mappings of resource + parameters to internal callbacks
 *
 * @returns Array of mappings
 */
function kendra_social_services_services_get_mappings() {
	$resources = array();
	$resources['activitystreams'] = array(); // stub for eventual activitystreams service
	$resources['albums'] = array(); // stub for eventual albums service
	$resources['groups'] = array(); // stub for eventual groups service
	$resources['mediaItems'] = array(); // stub for eventual mediaItems service
	$resources['people'] = array(); // stub for eventual people service
	$resources['profiles'] = array();  // stub for eventual profiles service
	$resources['smartfilters'] = array(); // stub for eventual Smart Filters service

	/**
	 * accounts service
	 */
	$resources['accounts'] = array(
        'index' => array(
          'enabled' => 1,
	),
	'create' => array(
          'enabled' => 1,
	),
	'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
	),
        'create' => array(
          'enabled' => 1,
	),
        'update' => array(
          'enabled' => 1,
	),
        'delete' => array(
          'enabled' => 1,
	),
        'index' => array(
          'enabled' => 1,
	),
	),
	'actions' => array(
	'connect' => array(
        'access arguments' => array('access kendra social connect'),
        'callback' => '_kss_social_connect',
        'args' => array(
	array(
            'name' => 'domain',
            'optional' => FALSE,
            'source' => array('data' => 'domain'),
            'type' => 'string',
            'description' => 'The top-most authoritative domain for the social network to connect to',
	),
	),
          'enabled' => 1,
	),
	),
	);

	/**
	 * social service
	 */
	$resources['social'] = array(
      'publish' => array(
		'enabled' => 1,
	),

	'actions' => array(
      'publish' => array(
        'access arguments' => array('access kendra social connect'),
        'callback' => '_kss_social_publish', // stub for eventual social network publish service
        'args' => array(
	array(
            'name' => 'domain',
            'optional' => FALSE,
            'source' => array('data' => 'domain'),
            'type' => 'string',
            'description' => 'The top-most authoritative domain for the social network to publish to',
	),
	array(
            'name' => 'content',
            'optional' => FALSE,
            'source' => array('data' => 'content'),
            'type' => 'string',
            'description' => 'Content to post to social network',
	),
	),
	'enabled' => 1,
	),
	),
	);

	/**
	 * user service
	 */
	$resources['user'] = array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
	),
        'create' => array(
          'enabled' => 1,
	),
        'update' => array(
          'enabled' => 1,
	),
        'delete' => array(
          'enabled' => 1,
	),
        'index' => array(
          'enabled' => 1,
	),
	),
      'actions' => array(
        'login' => array(
          'enabled' => 1,
	),
        'logout' => array(
          'enabled' => 1,
	),
        'register' => array(
          'enabled' => 1,
	),
	),
	);

	return $resources;
}
