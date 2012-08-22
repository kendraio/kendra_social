<?php
/**
 * @file
 * kendra_social_boxes.features.inc
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function kendra_social_boxes_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implementation of hook_node_info().
 */
function kendra_social_boxes_node_info() {
  $items = array(
    'album' => array(
      'name' => t('Albums'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
