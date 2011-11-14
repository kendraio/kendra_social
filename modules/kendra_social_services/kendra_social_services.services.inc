<?php
/**
 * @file
 * kendra_social_services.services.inc
 */
/**
 * Implementation of hook_default_services_endpoint().
 */
function kendra_social_services_default_services_endpoint() {
	$export = array();

	$kendra_social_services_endpoint_name = variable_get('KENDRA_SOCIAL_SERVICES_ENDPOINT_NAME', 'saracen');
	$kendra_social_services_endpoint_prefix = variable_get('KENDRA_SOCIAL_SERVICES_ENDPOINT_PREFIX', 'saracen/0.1');

	$endpoint = new stdClass;
	$endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
	$endpoint->api_version = 3;
	$endpoint->name = $kendra_social_services_endpoint_name;
	$endpoint->server = 'rest_server';
	$endpoint->path = $kendra_social_services_endpoint_prefix;
	$endpoint->authentication = array(
    'services' => 'services',
	);
	$endpoint->server_settings = array(
    'rest_server' => array(
      'formatters' => array(
        'bencode' => TRUE,
        'json' => TRUE,
        'php' => TRUE,
        'rss' => TRUE,
        'xml' => TRUE,
        'yaml' => TRUE,
        'jsonp' => TRUE,
	),
      'parsers' => array(
        'application/json' => TRUE,
        'application/vnd.php.serialized' => TRUE,
        'application/x-www-form-urlencoded' => TRUE,
        'application/x-yaml' => TRUE,
        'multipart/form-data' => TRUE,
	),
	),
	);
	$endpoint->resources = array(
    'accounts' => array(
      'actions' => array(
        'connect' => array(
          'enabled' => 1,
	),
	),
	),
	    'user' => array(
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
	),
	);
	$endpoint->debug = 1;
	$export[$kendra_social_services_endpoint_name] = $endpoint;

	return $export;
}
