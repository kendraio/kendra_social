<?php
/**
 * @file
 * kendra_social_import.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function kendra_social_import_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass;
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'fb_profile_import';
  $feeds_importer->config = array(
    'name' => 'Facebook profile import',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsJSONPathParser',
      'config' => array(
        'context' => '',
        'sources' => array(),
        'debug' => array(),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsSPARQLProcessor',
      'config' => array(
        'sparql_store_name' => 'fb_profile',
        'mappings' => array(),
        'update_existing' => '0',
        'input_format' => 'plain_text',
      ),
    ),
    'content_type' => 'facebook_profile',
    'update' => 0,
    'import_period' => '86400',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 1,
  );
  $export['fb_profile_import'] = $feeds_importer;

  return $export;
}
