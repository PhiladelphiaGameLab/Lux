<?php
// Mostly Done for now
include_once("lux-functions.php");
include_once("db.php");
include_once("output.php");

class Auth{

	private $client_doc;

	function __construct(){
		// test to make sure that access code is legit
		$LuxFunctions = new LuxFunctions();
		$OUTPUT = new Output(); 
		$access_token = $LuxFunctions->is_avail("access_token");
		$DB = new db();
		$clientInfo = $DB->selectCollection("ClientInfo");
		$this->client_doc = $clientInfo->findOne(array("access_token" => $access_token));
		if(!isset($this->client_doc)){
			$OUTPUT->error("Access Code is invalid or has Expired");
		}
	}
	function getClientId(){
		return $this->client_doc["_id"];
	}
	function getClientInfo(){
		return $this->client_doc;
	}
	function getClientGroups(){
		return $this->client_doc["groups"];
	}
}

class AuthLogin{

	private $client_doc;
	
	function __construct($identifier){

		$OUTPUT = new Output();
		$DB = new db();
		$clientInfo = $DB->selectCollection("ClientInfo");
		
		// find user by the identifier
		// if the user is found
		// return the users current access token if one exists
		// otherwise, create a new access token and return that
		// do other stuff
	}


}

class OAuth{

	static private $client_id;
	static private $client_secret;
	static private $redirect_url;
	static private $grant_type;


	static function getURL($service){
		switch($service){
			case "Google":
				return "";
		}
	}

	static function saveToDb(){
		// do stuff to save the user to the database
	}




}
?>
