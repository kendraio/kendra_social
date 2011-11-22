<?php
/**
 * @file
 * kendra_social_import.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function kendra_social_import_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function kendra_social_import_node_info() {
  $items = array(
    'facebook_profile' => array(
      'name' => t('Facebook Profile'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
