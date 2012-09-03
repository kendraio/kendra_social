<?php
/**
 * @file
 * kendra_social_profile.relation_type_default.inc
 */

/**
 * Implements hook_relation_default_relation_types().
 */
function kendra_social_profile_relation_default_relation_types() {
  $export = array();

  $relation_type = new stdClass();
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'album_contains';
  $relation_type->label = 'Album contains';
  $relation_type->reverse_label = 'Album member of';
  $relation_type->directional = 1;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 0;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 2;
  $relation_type->source_bundles = array(
    0 => 'node:album',
  );
  $relation_type->target_bundles = array(
    0 => 'node:mediaitem',
  );
  $export['album_contains'] = $relation_type;

  $relation_type = new stdClass();
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'album_user';
  $relation_type->label = 'Album user';
  $relation_type->reverse_label = 'Album user of';
  $relation_type->directional = 1;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 0;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 2;
  $relation_type->source_bundles = array(
    0 => 'node:album',
  );
  $relation_type->target_bundles = array(
    0 => 'user:user',
  );
  $export['album_user'] = $relation_type;

  $relation_type = new stdClass();
  $relation_type->disabled = FALSE; /* Edit this to true to make a default relation_type disabled initially */
  $relation_type->api_version = 1;
  $relation_type->relation_type = 'derivative';
  $relation_type->label = 'Derivative';
  $relation_type->reverse_label = 'Derivative of';
  $relation_type->directional = 1;
  $relation_type->transitive = 0;
  $relation_type->r_unique = 0;
  $relation_type->min_arity = 2;
  $relation_type->max_arity = 2;
  $relation_type->source_bundles = array(
    0 => 'node:mediaitem',
  );
  $relation_type->target_bundles = array(
    0 => 'node:derivative',
  );
  $export['derivative'] = $relation_type;

  return $export;
}
