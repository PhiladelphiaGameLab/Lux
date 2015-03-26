<?php
// Good enough for now
include_once("lux-functions.php");
include_once("output.php");
include_once("db.php");
class Output{
	
	private $array;	
	private $output;
	function __construct(){
		$this->time = microtime(true);
		$this->array = array();
		$this->output = array(
				"status"=>array(
					"code" => 1,
					"status"=>"OK",
					"message"=>"success"
				),
				"meta"=>array(
					"timestamp" => time(),
					"version"=>"4.0",
					"API"=>"lux"
				),
				"request"=>array(
					"method" => $_SERVER["REQUEST_METHOD"],
					"time"=>$_SERVER["REQUEST_TIME"],
					"url"=> $_SERVER["REQUEST_URI"]
				)
			);
				
	}	
	function success($code, $data, $results){
		switch($code){
			case 0:
				$this->output["status"]["request"]["type"] = "update";
				break;
			case 1:
				$this->output["status"]["request"]["type"] = "query";
				break;
			case 2:
				$this->output["status"]["request"]["type"] = "remove";
				break;
		}
		$this->output["status"]["request"]["code"] = $code;
		if(!is_array($data)){
			$data = iterator_to_array($data);
		}
		if(!is_array($results)){
			$results = iterator_to_array($results);
		}
		$this->output["data"] =  $data;
		$this->output["response"] = $results;
		$this->output["request"]["elapsed"] = (microtime(true) - $this->time)*1000;
		echo json_encode($this->output);	
	}
	function error($code, $message){
		$this->output["status"]["code"] = 0;
		$this->output["status"]["status"] = "error";
		$this->output["error"]["code"] = $code;
		$this->output["status"]["message"] = $message;
		switch($code){
			case 0:
				$this->output["error"]["status"] = "Required Parameter was not found";
		}
		$this->output["request"]["elapsed"] = (microtime(true) - $this->time)*1000;
		echo json_encode($this->output);	
		die();
	}
}
?>
