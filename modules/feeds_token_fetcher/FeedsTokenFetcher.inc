<?php

class FeedsTokenFetcherResult extends FeedsFetcherResult {
  protected $url;

  public function __construct($url = NULL) {
    $this->url = $url;
    parent::__construct('');
  }
  public function getRaw() {
    feeds_include_library('http_request.inc', 'http_request');
    $result = http_request_get($this->url);
    watchdog('fetcher debug', '<pre>'. print_r($this->url, TRUE) .'</pre>');
    if (!in_array($result->code, array(200, 201, 202, 203, 204, 205, 206))) {
      throw new Exception(t('Download of @url failed with code !code.', array('@url' => $this->url, '!code' => $result->code)));
    }
	  watchdog('fetcher debug', '<pre>'. print_r($this->url, TRUE) .'</pre>');
    return $this->sanitizeRaw($result->data);
  }
}


class FeedsTokenFetcher extends FeedsFetcher {

  public function fetch(FeedsSource $source) {
	  $node = node_load($source->feed_nid);
	  $url = token_replace($this->config['url'], array('node' => $node));
    return new FeedsTokenFetcherResult($url);
  }
  public function clear(FeedsSource $source) {
	  $node = node_load($source->feed_nid);
	  $url = token_replace($this->config['url'], array('node' => $node));
    feeds_include_library('http_request.inc', 'http_request');
    http_request_clear_cache($url);
  }

  public function configDefaults() {
    return array(
      'url' => '',
    );
  }

  public function configForm(&$form_state) {
    $form = array();
    $form['url'] = array(
      '#type' => 'textfield',
      '#title' => t('Tokenized URL'),
      '#description' => t('A URL that may contain tokens that are substituted from the node fields before fetching.'),
      '#default_value' => $this->config['url'],
    );
 	  $form['token_help'] = array(
 	    '#type' => 'fieldset',
 	    '#title' => t('Replacement patterns'),
 	    '#collapsible' => TRUE,
 	    '#collapsed' => TRUE,
 	  );
	 	$tokens = token_info();
	  foreach ($tokens['tokens']['node'] as $token => $info) {
		  $tokes[] = array("[node:{$token}]", $info['name'], $info['description']);
	  }
		$form['token_help']['info'] = array(
		  '#markup' => theme('table', array('header' => array('token', 'name', 'description'), 'rows' => $tokes)
		  ), 
		);
		
 	  
    return $form;
  }
}

