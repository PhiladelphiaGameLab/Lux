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
		if(is_array(func_get_args(0))){
			echo json_encode(func_get_args(0));
		}else{
			echo json_encode(func_get_args());	
		}
	}
	function error(){
		echo json_encode(func_get_args());	
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
