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
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
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
  $endpoint->resources = array(
    'mediaItems' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => '1',
        ),
        'update' => array(
          'enabled' => '1',
        ),
        'delete' => array(
          'enabled' => '1',
        ),
        'index' => array(
          'enabled' => '1',
        ),
        'create' => array(
          'enabled' => '1',
        ),
      ),
      'relationships' => array(
        'annotations' => array(
          'enabled' => '1',
        ),
      ),
      'targeted_actions' => array(
        'rate' => array(
          'enabled' => '1',
        ),
        'tag' => array(
          'enabled' => '1',
        ),
        'annotate' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'search' => array(
      'operations' => array(
        'index' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'boxes' => array(
      'operations' => array(
        'index' => array(
          'enabled' => '1',
        ),
        'retrieve' => array(
          'enabled' => '1',
        ),
        'create' => array(
          'enabled' => '1',
        ),
      ),
      'targeted_actions' => array(
        'add_item' => array(
          'enabled' => '1',
        ),
        'user' => array(
          'enabled' => '1',
        ),
        'delete_item' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'accounts' => array(
      'actions' => array(
        'connect' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'profiles' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'social' => array(
      'actions' => array(
        'publish' => array(
          'enabled' => '1',
        ),
      ),
    ),
    'user' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => '1',
        ),
        'create' => array(
          'enabled' => '1',
        ),
        'update' => array(
          'enabled' => '1',
        ),
        'delete' => array(
          'enabled' => '1',
        ),
        'index' => array(
          'enabled' => '1',
        ),
      ),
      'actions' => array(
        'login' => array(
          'enabled' => '1',
        ),
        'logout' => array(
          'enabled' => '1',
        ),
        'register' => array(
          'enabled' => '1',
        ),
      ),
    ),
  );
  $endpoint->debug = 1;
  $export['saracen'] = $endpoint;

  return $export;
}
