<?php
/**
 * @file
 * kendra_social_profile.features.inc
 */

/**
 * Implementation of hook_default_profile2_type().
 */
function kendra_social_profile_default_profile2_type() {
  $items = array();
  $items['facebook'] = entity_import('profile2_type', '{
    "userCategory" : false,
    "userView" : false,
    "type" : "facebook",
    "label" : "Facebook",
    "weight" : "0",
    "data" : { "registration" : 0, "use_page" : 1 },
    "rdf_mapping" : []
  }');
  $items['main'] = entity_import('profile2_type', '{
    "userCategory" : false,
    "userView" : false,
    "type" : "main",
    "label" : "Main profile",
    "weight" : "0",
    "data" : { "registration" : 1, "use_page" : 1 },
    "rdf_mapping" : []
  }');
  return $items;
}
