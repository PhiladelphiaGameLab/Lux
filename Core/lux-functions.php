<?php
include_once("output.php");
include_once("db.php");
include_once("output.php");

// functionally working
class LuxFunctions{

	private $OUTPUT;
	private $parameters;
	private $docSet;

	function LuxFunctions(){
		$this->OUTPUT = new Output();
		$this->parameters = array();
		$this->setArray();
		$this->docSet = false;
		if(!isset($_SESSION)){
			//session_start();
		}
	}

	function getParameters(){
		return $this->parameters;
	}
	function checkGroups($documentGroup, $clientGroups){
		return array_search($documentGroup, $clientGroups);
		// check that the group permission includes "copyable"
	}

	function setArray(){
		$decoded = json_decode(file_get_contents('php://input'), true);
		if(is_array($decoded)){
			foreach($decoded as $key => $value){
				$_POST[$key] = $value;
				$this->parameters[strtolower($key)] = $value;
			}
		}
		if(is_array($_POST)){
			foreach($_POST as $key => $value){
				$this->parameters[strtolower($key)] = $value;
			}
		}
		if(is_array($_GET)){
			foreach($_GET as $key => $value){
				$this->parameters[strtolower($key)] = $value;
			}
		}
		if(isset($_SESSION) && is_array($_SESSION)){
			foreach($_SESSION as $key => $value){
				$this->parameters[strtolower($key)] = $value;
			}
		}
	}

	function setDocument($doc){
		if(!$this->docSet){
			$this->docSet = true;
			if(is_array($doc)){
				foreach($doc as $key => $value){
					$this->parameters[strtolower($key)] = $value;
				}
			}
		}
	}

	function is_avail($var, $doc=""){
		$this->setDocument($doc);
		if(!isset($this->parameters[strtolower($var)])){
			return false;
		}else{
			return true;
		}
	}

	function fetch_avail($var, $die=true, $doc=""){
		$this->setDocument($doc);
		if(!isset($this->parameters[strtolower($var)])){
			if($die){
				$this->OUTPUT->error("Required Variable is undefined", array("variable" => $var));
			}else{
				return false;
			}
		}
		return $this->parameters[strtolower($var)];
	}

	function is_get($var, $die=true){
		if(!isset($_GET[$var])){
			if($die){
				$this->OUTPUT->error("Required Variable is undefined", array("variable" => $var));
			}else{
				return false;
			}
		}
		return $_GET[$var];
	}

	function is_post($var, $die=true){
		if(!isset($_POST[$var])){
			if($die){
				$this->OUTPUT->error("Required Variable is undefined", array("variable" => $var));
			}else{
				return false;
			}
		}
		return $_POST[$var];
	}
}


?>
  
