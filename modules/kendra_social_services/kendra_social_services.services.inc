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

  $endpoint = new stdClass();
  $endpoint->disabled = FALSE;
  $endpoint->api_version = 3;
  $endpoint->name = 'saracen';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'saracen/0.1';
  $endpoint->authentication = array(
    'services' => 'services',
  );
  $endpoint->server_settings = array(
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
  );

  /**
  * all service resources defined by KSS are enabled by default
  */
  $endpoint_resources = array();
  $resources = kendra_social_services_services_get_mappings();

  foreach($resources as $controller => $resource) {
      foreach($resource as $resource_type => $resource_set) {
          if (in_array($resource_type, array('operations', 'actions', 'relationships', 'targeted_actions'))) {
              foreach($resource_set as $action => $resource_array) {
                  if (!isset($resource_array['enabled']) || $resource_array['enabled']) {
                      $endpoint_resources[$controller][$resource_type][$action]['enabled'] = TRUE;
                  }
              }
          }
      }
  }
  $endpoint->resources = $endpoint_resources;
  $endpoint->debug = 1;
  $export['saracen'] = $endpoint;
  return $export;
}

