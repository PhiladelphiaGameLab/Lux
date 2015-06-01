<?php
include_once("db.php");

		$DB = new Db();
		$collection = $DB->selectCollection("test");
		$query = array("eyecolor" => "blue");
		$checkcursor = $collection->find($query);
		$retdoc = $checkcursor;
                $retquery = array('$or' => array());
		foreach($checkcursor as $value){
    			$retquery['$or'][] = array("_id" => $value['_id']);
		}
		if(empty($retquery['$or'][0])){
                        echo "YAY!";
                }
		var_dump($retquery);

		//$checkcursor = $collection->find($retquery);
		
		foreach($checkcursor as $value){
			var_dump($value);
		}
		
		

?>
  
