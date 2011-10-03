<?php
/**
 * @file
 * kendra_social_services.features.inc
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function kendra_social_services_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
}
