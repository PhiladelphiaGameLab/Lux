<?php
// This is also probably done
include_once("lux-functions.php");
include_once("output.php");
include_once("db.php");

/*
Params in:
	$_POST = array(
		_id of document (optional),
		doc for document,
		query for mongo query,
		priority (optional)
	)
	$params = array(
		enqueue, write this update to the Queue
		collectionName for collection, 		
	)
Ouput: 
	updated document(s)
Functionality:
	Takes a full document, or a document update, and either creates a new document or updates an existing document.

TODO:
	Functionality to Link to SN node or Unlink or Create Node and Link or Delete Node and Unlink
	Can be added to the Event Triggers Pub/Sub as well as the Live Pub/Sub
*/

function upsert($params){
	$options = array("new" => true);
	$db = new Db();
	$collection = $db->selectCollection($params["collectionName"]);	
	$OUTPUT = new Output();
	$AUTH = new Auth();
	$LuxFunctions = new LuxFunctions();
	$fields = null;	
	// check if the document paramter is specified
	// do upserting stuff:
		
	if($LuxFunctions->is_avail("doc")){
		$LuxFunctions->setDocument($LuxFunctions->fetch_avail("doc"));
	}
	if($LuxFunctions->is_avail("id") || $LuxFunctions->is_avail("query")){
		if(!$LuxFunctions->is_avail("doc")){
			$options["remove"] = true;
			$update = null;	
		}else{
			$options["upsert"] = true;
			$update = $LuxFunctions->fetch_avail("doc");
		}
		// update based on _id or Query
		if($LuxFunctions->is_avail("id")){
			$id = $LuxFunctions->fetch_avail("id");
			$query = array("_id" => new MongoId($id));
		}else if($LuxFunctions->is_avail("query")){
			$query = $LuxFunctions->fetch_avail("query");
		}
		$checkcursor = $collection->find($query);
		$hasPermssion = true;
		foreach($checkcursor as $value){
			if(!($LuxFunctions->checkGroups($AUTH->getGroups(), $value["group_id"]))){
				$hasPermission = false;
			}
		}
	}else{
		$options["insert"] = true;
		$update = $LuxFunctions->fetch_avail("doc", false);
		$query = null;
		$hasPermissions = true;
	}
	if($hasPermission){
		$result = $collection->findAndModify($query, $update, $fields, $options);
		$OUTPUT->success("Successfully updated document", $result);
		if($params["enqueue"]){
			$db->enQueue($LuxFunctions->fetch_avail("id"), $AUTH, $LuxFunctions->fetch_avail("doc", false), $params["priority"]);
		}
	}else{
		$OUTPUT->error("User does not have permission to required group", $group);
	}
}

//$params = array();
//$params["collectionName"] = "Assets";
//$params["doc"] = "this is a test";
//$params["query"] = "test";

//upsert($params);
?>
