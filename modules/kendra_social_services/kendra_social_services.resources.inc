<?php
/**
 * @file
 * Code for the Kendra Social Services feature.
 */

/**
 * define the mappings of resource + parameters to internal callbacks
 *
 * @returns Array of mappings
 */
function kendra_social_services_services_get_mappings() {
	ob_start();
	require DRUPAL_ROOT . '/' . drupal_get_path('module', 'kendra_social_services') . '/services/kendra_api_method.methods.json' ;
	$services_definition = ob_get_clean();
	
	$resources = json_decode($services_definition, true); // convert JSON to associative arrays
	//watchdog('service debug', '<pre>' . print_r($resources, TRUE) . '</pre>');
	// TODO: Move these service definitions into JSON format 
	
	$resources['mediaItems']['relationships']['recommendations'] = array(
		'access arguments' => array('access content'),
		'access callback' => '_kendra_saracen_trial_mod_default_access_callback',
		'access arguments append' => FALSE,
		'args' => array(
			array(
				'name' => 'id',
				'description' => 'identifier',
				'source' => array('path' => '0'),
				'type' => 'int',
				'optional' => FALSE,
		  )
		),
		'callback' => '_kendra_saracen_trial_mod_media_recommendations',
	);
	$resources['user']['relationships']['watched'] = array(
		'access arguments' => array('access content'),
		'access callback' => '_kendra_saracen_trial_mod_default_access_callback',
		'access arguments append' => FALSE,
		'args' => array(
			array(
				'name' => 'id',
				'description' => 'identifier',
				'source' => array('path' => '0'),
				'type' => 'int',
				'optional' => FALSE,
		  )
		),
		'callback' => '_kendra_saracen_trial_mod_user_watched',
	); 
	$resources['mediaItems']['targeted_actions']['startWatching'] = array(
		'access arguments' => array('access content'),
		'access callback' => '_kendra_saracen_trial_mod_default_access_callback',
		'access arguments append' => FALSE,
		'args' => array(
			array(
				'name' => 'id',
				'description' => 'identifier',
				'source' => array('path' => '0'),
				'type' => 'int',
				'optional' => FALSE,
		  )
		),
		'callback' => '_kendra_saracen_trial_mod_start_watching_mediaitem',
	);
	$resources['mediaItems']['targeted_actions']['activity'] = array(
		'access arguments' => array('access content'),
		'access callback' => '_kendra_saracen_trial_mod_default_access_callback',
		'access arguments append' => FALSE,
		'args' => array(
			array(
				'name' => 'id',
				'description' => 'identifier',
				'source' => array('path' => '0'),
				'type' => 'int',
				'optional' => FALSE,
		  ),
			array(
				'name' => 'publish',
				'description' => 'is watching activity published',
				'source' => array('data' => 'publish'),
				'type' => 'int',
				'optional' => FALSE,
		  ),
		),
		'callback' => '_kendra_saracen_trial_mod_activity_track_mediaitem',
	);
	$resources['mediaItems']['targeted_actions']['stopWatching'] = array(
		'access arguments' => array('access content'),
		'access callback' => '_kendra_saracen_trial_mod_default_access_callback',
		'access arguments append' => FALSE,
		'args' => array(
			array(
				'name' => 'id',
				'description' => 'identifier',
				'source' => array('path' => '0'),
				'type' => 'int',
				'optional' => FALSE,
		  )
		),
		'callback' => '_kendra_saracen_trial_mod_stop_watching_mediaitem',
	); 
	$resources['watchingNow'] = array(
		'operations' => array(
			'index' => array(
				'access arguments' => array('access content'),
				'access arguments append' => FALSE,
				'access callback' => '_kendra_saracen_trial_mod_default_access_callback',
				'callback' => '_kss_watching_now',
				'args' => array(
					array(
						'name' => 'limit',
						'source' => array('param' => 'limit'),
						'type' => 'int',
						'optional' => TRUE,
						'default value' => 5,
					),
					array(
						'name' => 'who',
						'source' => array('param' => 'who'),
						'type' => 'string',
						'optional' => TRUE,
						'default value' => 'friends',
					),
					array(
						'name' => 'sort',
						'source' => array('param' => 'sort'),
						'type' => 'string',
						'optional' => TRUE,
						'default value' => 'affinity',
					),
					array(
						'name' => 'filter3d',
						'source' => array('param' => 'filter3d'),
						'type' => 'string',
						'optional' => TRUE,
						'default value' => 'both',
					),
					array(
						'name' => 'friends',
						'source' => array('param' => 'friends'),
						'type' => 'string',
						'optional' => TRUE,
						'default value' => 'yes',
					),
				),
			),
		),
	);
	$resources['annotations'] = array(
		'operations' => array(
			'delete' => array(
				'access arguments' => array('access content'),
				'access callback' => '_kendra_saracen_trial_mod_default_access_callback',
				'access arguments append' => FALSE,
        'file' => array(
	        'type' => 'inc', 
	        'module' => 'kendra_saracen_trial_mod',
	        'name' => 'kendra_saracen_trial.extra',
	      ),
				'args' => array(
					array(
						'name' => 'id',
						'description' => 'identifier',
						'source' => array('path' => '0'),
						'type' => 'int',
						'optional' => FALSE,
				  ),
					array(
						'name' => 'data',
						'source' => array('path' => '1'),
						'description' => 'data with media identifier',
						'type' => 'string',
						'optional' => TRUE,
				  )
				),	
				'callback' => '_kss_annotation_delete',
				'enabled' => 1,
			),
			'retrieve' => array(
				'access arguments' => array('access content'),
        'file' => array(
	        'type' => 'inc', 
	        'module' => 'kendra_saracen_trial_mod',
	        'name' => 'kendra_saracen_trial.extra',
	      ),
				'access callback' => '_kendra_saracen_trial_mod_default_access_callback',
				'access arguments append' => FALSE,
				'args' => array(
					array(
						'name' => 'id',
						'description' => 'identifier',
						'source' => array('path' => '0'),
						'type' => 'int',
						'optional' => FALSE,
				  ),
				),
				'callback' => '_kss_annotation_retrieve',
				'enabled' => 1,
			),
		),
	);
	
	$resources['annotations']['targeted_actions']['delete'] = array(
		'access arguments' => array('access content'),
		'access callback' => '_kendra_saracen_trial_mod_default_access_callback',
		'access arguments append' => FALSE,
    'file' => array(
      'type' => 'inc', 
      'module' => 'kendra_saracen_trial_mod',
      'name' => 'kendra_saracen_trial.extra',
    ),
		'args' => array(
			array(
				'name' => 'id',
				'description' => 'identifier',
				'source' => array('path' => '0'),
				'type' => 'int',
				'optional' => FALSE,
		  ),
			array(
				'name' => 'media',
				'source' => array('data' => 'identifier'),
				'description' => 'media identifier',
				'type' => 'string',
				'optional' => TRUE,
		  )
		),	
		'callback' => '_kss_annotation_delete',
		'enabled' => 1,
	);
	//die(json_encode(print_r($resources,1)));
	return $resources;
}


/**
 * Kendra Social Default Service Callback
 */
function _kss_default_callback() {
	return "Kendra Social default callback";
}

/**
 * Kendra Social Services Default Access Callback
 * returns TRUE, allowing all access!!
 * @todo don't return true, allowing all access
 */
function _kss_default_access_callback($op, $args){
	watchdog('Kendra Social', 'verifying access for @op, @args', array('@op'=>$op, '@args'=>print_r($args,1)));

	return TRUE;
}
