<?php
/**
 * @file
 * kendra_saracen_components.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function kendra_saracen_components_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function kendra_saracen_components_node_info() {
  $items = array(
    'album' => array(
      'name' => t('Albums'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'annotation' => array(
      'name' => t('Annotation'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'derivative' => array(
      'name' => t('Derivative'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'mediaitem' => array(
      'name' => t('MediaItem'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
