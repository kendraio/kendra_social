<?php
/**
 * @file
 * kendra_social_profile.features.user_permission.inc
 */

/**
 * Implementation of hook_user_default_permissions().
 */
function kendra_social_profile_user_default_permissions() {
  $permissions = array();

  // Exported permission: edit any facebook profile
  $permissions['edit any facebook profile'] = array(
    'name' => 'edit any facebook profile',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'profile2',
  );

  // Exported permission: edit any main profile
  $permissions['edit any main profile'] = array(
    'name' => 'edit any main profile',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'profile2',
  );

  // Exported permission: edit own facebook profile
  $permissions['edit own facebook profile'] = array(
    'name' => 'edit own facebook profile',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'profile2',
  );

  // Exported permission: edit own main profile
  $permissions['edit own main profile'] = array(
    'name' => 'edit own main profile',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'profile2',
  );

  // Exported permission: view any facebook profile
  $permissions['view any facebook profile'] = array(
    'name' => 'view any facebook profile',
    'roles' => array(
      0 => 'anonymous user',
      1 => 'authenticated user',
    ),
    'module' => 'profile2',
  );

  // Exported permission: view any main profile
  $permissions['view any main profile'] = array(
    'name' => 'view any main profile',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
    ),
    'module' => 'profile2',
  );

  // Exported permission: view own facebook profile
  $permissions['view own facebook profile'] = array(
    'name' => 'view own facebook profile',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'profile2',
  );

  // Exported permission: view own main profile
  $permissions['view own main profile'] = array(
    'name' => 'view own main profile',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'profile2',
  );

  return $permissions;
}
