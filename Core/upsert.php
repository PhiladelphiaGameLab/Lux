<?php
// This is also probably done
include_once("lux-functions.php");
include_once("output.php");
include_once("db.php");
include_once("auth.php");
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
	//-------Setup for Upsert--------//
	$db = new Db();
	$collection = $db->selectCollection($params["collectionName"]);	
	$OUTPUT = new Output();
	//$AUTH = new Auth();
	$LuxFunctions = new LuxFunctions();
	$LuxFunctions->setDocument($params);
	$fields = null;	
	$hasPermission = true;
	if($LuxFunctions->is_avail("options")){
		$options = $LuxFunctions->fetch_avail("options");
		$options["new"] = true;
	}else{
		$options = array("new" => true);
	}
	//------------------------------//

	/* Check 1: Establish existance of Update Document */
	if($LuxFunctions->is_avail("doc")){ 
		$update = $LuxFunctions->fetch_avail("doc", false);
		$query = $update; 
		//set query in case doc is only passed information
	}
	
	/* Check 2: Establish existance of Document ID or Query */
	if($LuxFunctions->is_avail("id") || $LuxFunctions->is_avail("query")){
		if(!($LuxFunctions->is_avail("doc"))){ //if doc doesn't exist, remove
			$options["remove"] = true;
			$options["new"] = false;
			$update = null;	
		}else{ //if not, upsert
			$options["upsert"] = true;
			$update = $LuxFunctions->fetch_avail("doc");
		}
		
		/* Check 3: Determine ID or Query for query focus */
		if($LuxFunctions->is_avail("id")){ 
			//id takes priority
			$id = $LuxFunctions->fetch_avail("id");
			$query = array("_id" => new MongoId($id));
		}else if($LuxFunctions->is_avail("query")){
			$tempquery = $LuxFunctions->fetch_avail("query");
			if(is_array($tempquery)){
				$query = $tempquery;
			}else{
				$query = array($tempquery);
			}
		}
		
		/* Check 4: Determine whether or not the user has permission */
		$checkcursor = $collection->find($query);
		$retquery = array('$or' => array()); //query used for queue
		$hasPermission = true;
		/*foreach($checkcursor as $value){
			$retquery['$or'][] = array("_id" => $value['_id']); //setup queue query
			if(($LuxFunctions->checkGroups($value["group_id"], $AUTH->getClientGroups())) === false){
				$hasPermission = false;
			}
		}*/
		if(empty($retquery['$or'][0])){ //will only be true if there is a potential insert
			$params["insert"] = true;
		}
	}else{
		$options["insert"] = true;
		$update = $LuxFunctions->fetch_avail("doc", false);
		//$query = $update;
		$hasPermissions = true;
	}
	if(isset($params["SN"]) && $params["SN"]){
		// create a node if none exists 
	}
	if($hasPermission){
		if(isset($options["remove"]) && $options["remove"] == true){
			$removed = $collection->find($query);
			$result = $collection->remove(array("_id" => $removed["_id"]));
		}else if(isset($params["insert"]) && $params["insert"] == true){
			$result = $collection->insert($update);
		}else{
			$result = $collection->update($query, $update, $options); 
		}
		$OUTPUT->success("Successfully updated document", $result);
		if(isset($params["enqueue"]) && $params["enqueue"]){
			if(isset($options["remove"]) && $options["remove"] == true){ //add removed documents to the queue
				foreach($checkcursor as $value){
					$value["removed"] = true;
					$db->publish($value, $AUTH, $params["priority"]);
				}
			}else if(isset($params["insert"]) && $params["insert"] == true){ //add insert to the queue
				$db->publish($update, $AUTH, $params["priority"]);
			}else{
				$quecursor = $collection->find($retquery); 
				foreach($quecursor as $value){ //add each augmented document to the queue
					$db->publish($value, $AUTH, $params["priority"]);
				}
			}
		}
	}else{
		$OUTPUT->error("User does not have permission to required group", $AUTH->getClientGroups());
	}
}


?>
  
