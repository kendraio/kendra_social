<?php
/**
 * @file
 * kendra_social_profile.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function kendra_social_profile_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "relation" && $api == "relation_type_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function kendra_social_profile_views_api() {
  return array("version" => "3.0");
}
