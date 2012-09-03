<?php
/**
 * @file
 * kendra_social_profile.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function kendra_social_profile_user_default_permissions() {
  $permissions = array();

  // Exported permission: create facebook_profile content.
  $permissions['create facebook_profile content'] = array(
    'name' => 'create facebook_profile content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: create kendra_album content.
  $permissions['create kendra_album content'] = array(
    'name' => 'create kendra_album content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: create kendra_mediaitem content.
  $permissions['create kendra_mediaitem content'] = array(
    'name' => 'create kendra_mediaitem content',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: create mediaitem content.
  $permissions['create mediaitem content'] = array(
    'name' => 'create mediaitem content',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any facebook_profile content.
  $permissions['delete any facebook_profile content'] = array(
    'name' => 'delete any facebook_profile content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any kendra_album content.
  $permissions['delete any kendra_album content'] = array(
    'name' => 'delete any kendra_album content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any kendra_mediaitem content.
  $permissions['delete any kendra_mediaitem content'] = array(
    'name' => 'delete any kendra_mediaitem content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any mediaitem content.
  $permissions['delete any mediaitem content'] = array(
    'name' => 'delete any mediaitem content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own facebook_profile content.
  $permissions['delete own facebook_profile content'] = array(
    'name' => 'delete own facebook_profile content',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own kendra_album content.
  $permissions['delete own kendra_album content'] = array(
    'name' => 'delete own kendra_album content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own kendra_mediaitem content.
  $permissions['delete own kendra_mediaitem content'] = array(
    'name' => 'delete own kendra_mediaitem content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own mediaitem content.
  $permissions['delete own mediaitem content'] = array(
    'name' => 'delete own mediaitem content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any facebook_profile content.
  $permissions['edit any facebook_profile content'] = array(
    'name' => 'edit any facebook_profile content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any kendra_album content.
  $permissions['edit any kendra_album content'] = array(
    'name' => 'edit any kendra_album content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any kendra_mediaitem content.
  $permissions['edit any kendra_mediaitem content'] = array(
    'name' => 'edit any kendra_mediaitem content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any mediaitem content.
  $permissions['edit any mediaitem content'] = array(
    'name' => 'edit any mediaitem content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own facebook_profile content.
  $permissions['edit own facebook_profile content'] = array(
    'name' => 'edit own facebook_profile content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own kendra_album content.
  $permissions['edit own kendra_album content'] = array(
    'name' => 'edit own kendra_album content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own kendra_mediaitem content.
  $permissions['edit own kendra_mediaitem content'] = array(
    'name' => 'edit own kendra_mediaitem content',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own mediaitem content.
  $permissions['edit own mediaitem content'] = array(
    'name' => 'edit own mediaitem content',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
    ),
    'module' => 'node',
  );

  return $permissions;
}
