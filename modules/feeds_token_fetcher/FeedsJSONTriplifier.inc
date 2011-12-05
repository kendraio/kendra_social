<?php

class FeedsJSONTriplifier extends FeedsParser {

  /**
   * Implementation of FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
	  
    $result = new FeedsParserResult();

    $raw = trim($fetcher_result->getRaw());
    $array = json_decode($raw, TRUE);

  	//watchdog('pre flat', '<pre>'. print_r($array, TRUE) .'</pre>');

    $items = _array_flatten($array, $this->config['prefix'], $this->config['delimiter']);
    foreach ($items as $key => $item) {
	    $result->items[] = array(
	    	'key' => '#'. $key,
	      'value' => $item,
	      'source' => url('node/'. $source->feed_nid, array('absolute' => TRUE)),
	      'prefix' => $this->config['prefix'],
	    );
    }

  	//watchdog('feeds debug', '<pre>'. print_r($result, TRUE) .'</pre>');
    return $result;

	}
  public function configDefaults() {
    return array(
      'delimiter' => '/',
      'prefix' => 'facebook',
    );
  }
  public function configForm(&$form_state) {
    $form = array();
    $form['delimiter'] = array(
      '#type' => 'textfield',
      '#title' => t('Default delimiter'),
      '#default_value' => $this->config['delimiter'],
    );
    $form['prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Property prefix'),
      '#default_value' => $this->config['prefix'],
    );
    return $form;
  }
  public function getMappingSources() {
    return array(
	    'key' => array('name' => 'Key'),
	    'value' => array('name' => 'Value'),
	    'source' => array('name' => 'Source'),
	    'prefix' => array('name' => 'Prefix'),
	  );
  }

}  


function _array_flatten($array, $orgkey = '', $delimiter) { 
  if (!is_array($array)) {
	  return array($orgkey => $array);
  } else { 
	  $items = array();
	  foreach ($array as $key => $value) {
		  $items = array_merge($items, _array_flatten($value, $orgkey . $delimiter . $key, $delimiter));
	  }
	  return $items;
  }
}



