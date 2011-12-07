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
	
	//die(json_encode(print_r($resources,1)));
	return $resources;
}

/**
 * Kendra Social Default Service Callback
 */
function _kss_social_default_callback() {
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
