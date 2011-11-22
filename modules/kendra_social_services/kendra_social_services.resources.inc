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
	$resources['profiles'] = array();  // stub for eventual profiles service
	$resources['smartfilters'] = array(); // stub for eventual Smart Filters service
	$resources['contentitems'] = array(); // stub for eventual content service
	$resources['accounts'] = array(
    'actions' => array(
      'connect' => array(
        'access arguments' => array('access content'),
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
	),
	),
	);
	$resources['social'] = array(
    'actions' => array(
      'publish' => array(
        'access arguments' => array('access content'),
        'callback' => '_kss_social_publish', // stub for eventual social network publish service
        'args' => array(
	array(
            'name' => 'domain',
            'optional' => FALSE,
            'source' => array('data' => 'domain'),
            'type' => 'string',
            'description' => 'The top-most authoritative domain for the social network to connect to',
	),
	array(
            'name' => 'content',
            'optional' => FALSE,
            'source' => array('data' => 'content'),
            'type' => 'string',
            'description' => 'Content to post to social network',
	),
	),
	),
	),
	);
	return $resources;
}
