<?php

class FeedsSPARQLProcessor extends FeedsProcessor {
	
  public function entityType() { return 'none'; }
  protected function newEntity(FeedsSource $source) { return array(); }
  protected function entityLoad(FeedsSource $source, $entity_id) { return array(); }
  protected function entityValidate($entity) { return TRUE; }
  protected function entitySave($entity) { }
  protected function entityDeleteMultiple($entity_ids) {}

  // TODO: implement expire() ?

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'sparql_store_name' => 'name',
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = parent::configForm($form_state);
    $form['sparql_store_name'] = array(
      '#type' => 'textfield',
      '#title' => t('SPARQL store name'),
      '#default_value' => $this->config['sparql_store_name'],
    );
    return $form;
  }

	
  public function process(FeedsSource $source, FeedsParserResult $parser_result) {
	  //watchdog('process', 'Source: <pre>'. print_r($source, TRUE) .'</pre>');
	  //watchdog('process', 'Parse result: <pre>'. print_r($parser_result, TRUE) .'</pre>');
	
	  $store_name = $this->config['sparql_store_name'];
	  $mappings = array();
	  foreach ($this->config['mappings'] as $map) {
		  $mappings[$map['target']] = $map['source'];
	  }
    $items = array();
    foreach ($parser_result->items as $item) {
	    $items[trim($item[$mappings['graph']])][] = array(
	    	'subject' => trim($item[$mappings['subject']]),
	    	'predicate' => trim($item[$mappings['predicate']]),
	    	'object' => str_replace('"', '\"', trim($item[$mappings['object']])),
	    );
    }
	  //watchdog('process', 'Store: <pre>'. print_r($store_name, TRUE) .'</pre>');
	  //watchdog('process', 'Mappings: <pre>'. print_r($mappings, TRUE) .'</pre>');
	  //watchdog('process', 'Items: <pre>'. print_r($items, TRUE) .'</pre>');
    
    /*	
	  INSERT INTO <http://example.com/> {
		 <#foo> <bar> "baz" . 
		}
		*/
		//$store = _sparql_init_store($store_name);
		$store = sparql_get_store($store_name, SPARQL_ENDPOINT);
		//watchdog('process', 'Store: <pre>'. print_r($store, TRUE) .'</pre>');
		
		foreach ($items as $graph => $insert) { 
			$triples = '';
			foreach ($insert as $item) {
				$triples .= "  <{$item['subject']}> <{$item['predicate']}> \"{$item['object']}\" .\n";
			}
		  $query = "INSERT INTO <{$graph}> {\n". $triples ."}\n";
			//watchdog('process', 'Graph: <pre>'. print_r($graph, TRUE) .'</pre>');
			//watchdog('process', 'Insert: <pre>'. print_r($insert, TRUE) .'</pre>');
			watchdog('SPARQL Feed', 'Query: <pre>'. htmlentities(print_r($query, TRUE)) .'</pre>');
			$store->query($query);
			if ($errs = $store->getErrors()) {
			  watchdog('SPARQL Feed', 'Error: <pre>'. print_r($errs, TRUE) .'</pre>');
			}
		}	
			
	}

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();
    $targets += array(
      'graph' => array(
        'name' => t('Graph'),
      ),
      'subject' => array(
        'name' => t('Subject'),
      ),
      'predicate' => array(
        'name' => t('Predicate'),
      ),
      'object' => array(
        'name' => t('Object'),
      ),
    );

    return $targets;
  }

}
