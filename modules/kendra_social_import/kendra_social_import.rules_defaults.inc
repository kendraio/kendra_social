<?php
/**
 * @file
 * kendra_social_import.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function kendra_social_import_default_rules_configuration() {
  $items = array();
  $items['rules_import_facebook_profile_when_user_account_is_created'] = entity_import('rules_config', '{ "rules_import_facebook_profile_when_user_account_is_created" : {
      "LABEL" : "Import Facebook profile when user account is created",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "user_insert" ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "node",
              "param_type" : "facebook_profile",
              "param_title" : "Facebook profile for [account:name]",
              "param_author" : [ "account" ]
            },
            "PROVIDE" : { "entity_created" : { "entity_created" : "Created entity" } }
          }
        }
      ]
    }
  }');
  return $items;
}
