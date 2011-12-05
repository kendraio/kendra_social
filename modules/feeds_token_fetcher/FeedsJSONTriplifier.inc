<?php

class FeedsJSONTriplifier extends FeedsParser {

  /**
   * Implementation of FeedsParser::parse().
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
	  
    $result = new FeedsParserResult();

    $raw = trim($fetcher_result->getRaw());
    $array = json_decode($raw, TRUE);

    foreach ($array as $key => $item) {
      $result->items[] = array(
      	'key' => $key,
				'value' => print_r($item, TRUE),
      );
    }
  	watchdog('feeds debug', '<pre>'. print_r($result, TRUE) .'</pre>');
    return $result;

	}
  public function getMappingSources() {
    return array(
	    'key' => array('name' => 'Key'),
	    'value' => array('name' => 'Value'),
	  );
  }

}