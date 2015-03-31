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
	function success($code, $data, $results=null){
		switch($code){
			case 0:
				$this->output["status"]["request"]["type"] = "update";
				break;
			case 1:
				$this->output["status"]["request"]["type"] = "query";
				$this->output["response"] = $results;
				break;
			case 2:
				$this->output["status"]["request"]["type"] = "remove";
				$this->output["response"] = $results;
				break;
		}
		$this->output["status"]["request"]["code"] = $code;
		if(!is_array($data) && !is_null($data)){
			$data = iterator_to_array($data);
			$this->output["data"] =  array_values($data);
		}else if(!is_null($data)){
			$this->output["data"] =  $data;
		}
		if(!is_array($results) && !is_null($results)){
			$results = iterator_to_array($results);
			$this->output["results"] =  array_values($results);
		}else if(!is_null($results)){
			$this->output["results"] =  $results;
		}
		$this->output["request"]["execution_time"] = (microtime(true) - $this->time)*1000;
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
				break;
			case 1:
				$this->output["error"]["status"] = "Access Token is invalid";
				break;
			case 2:
				$this->output["error"]["status"] = "Invalid use of API call";
				break;
			case 3:
				$this->output["error"]["status"] = "Permission Level Denied";
				break;
		}
		$this->output["request"]["execution_time"] = (microtime(true) - $this->time)*1000;
		echo json_encode($this->output);	
		die();
	}
}
?>
