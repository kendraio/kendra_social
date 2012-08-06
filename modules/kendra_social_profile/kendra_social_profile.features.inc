<?php
/**
 * @file
 * kendra_social_profile.features.inc
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function kendra_social_profile_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "relation" && $api == "relation_type_default") {
    return array("version" => "1");
  }
}

/**
 * Implementation of hook_views_api().
 */
function kendra_social_profile_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}
