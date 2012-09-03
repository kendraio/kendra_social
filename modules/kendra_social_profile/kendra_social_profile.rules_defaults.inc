<?php
/**
 * @file
 * kendra_social_profile.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function kendra_social_profile_default_rules_configuration() {
  $items = array();
  $items['rules_import_facebook_profile_on_user_create'] = entity_import('rules_config', '{ "rules_import_facebook_profile_on_user_create" : {
      "LABEL" : "import facebook profile on user create",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "user_insert" ],
      "DO" : []
    }
  }');
  return $items;
}
