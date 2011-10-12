<?php

/**
 * Implementation of hook_default_box().
 */
function kendra_social_config_default_box() {
  $export = array();

  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'welcome';
  $box->plugin_key = 'kendra_social';
  $box->title = 'Welcome to Kendra Social';
  $box->description = 'Welcome box';
  $box->options = array(
    'body' => array(
      'value' => 'Kendra Social is a Drupal 7 distribution for social networking. [Fork this project on GitHub](https://github.com/kendrainitiative/kendra_social).',
      'format' => 'markdown',
    ),
  );
  $export['welcome'] = $box;

  return $export;
}
