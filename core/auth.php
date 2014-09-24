<?php
// Mostly Done for now
include_once("lux-functions.php");
include_once("db.php");
include_once("output.php");

class Auth{

	private $client_doc;

	function __construct(){
		// test to make sure that access code is legit
		$access_token = is_avail("access_token");
		$DB = new db();
		$clientInfo = $DB->selectCollection("ClientInfo");
		$client_doc = $clientInfo->findOne(array("access_token" => $access_token));
		if(!isset($client_doc)){
			$OUTPUT->error("Access Code is invalid or has Expired");
		}
	}
	function getClientId(){
		return $client_doc["_id"];
	}
	function getClientInfo(){
		return $client_doc;
	}
	function getClientGroups(){
		return $client_doc["groups"];
	}
}

$AUTH = new Auth();
















?>
