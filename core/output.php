<?php
// Good enough for now

class Output{
	
	private $array = array();	

	function success(){
		print_r(func_get_args());	
	}

	function error(){
		print_r(func_get_args());	
		die();
	}

	function addToPrintArray(){
		array_push($array, func_get_args());
	}

	function printArray(){
		$this->success(json_encode($array));
	}
}

$OUTPUT = new Output();
?>
