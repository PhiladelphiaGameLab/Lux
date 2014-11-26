<?php
// Good enough for now
include_once("lux-functions.php");
include_once("output.php");
include_once("db.php");

class Output{
	
	private $array;	

	function __construct(){
		$this->array = array();
	}	

	function success(){
		echo "<pre>";
		echo json_encode(func_get_args());	
		echo "\n\n</pre>";
	}

	function error(){
		echo "<pre>";
		echo json_encode(func_get_args());	
		echo "\n\n</pre>";
		die();
	}

	function addToPrintArray(){
		array_push($this->array, func_get_args());
	}

	function printArray(){
		$this->success($this->array);
	}
}

?>
