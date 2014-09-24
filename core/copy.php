<?php

require('auth.php');
require('db.php');
require('output.php');

/*
Params in:
	$params = array(
		updates
		collection
		enQueue, put into a queue
		R
			newGroup
			updates
			collection
			enQueue
			query
			id
	)
	query || id (for original Document);
Ouput: 
	new group document        
Functionality:
        creates a new group document
	copies the old group into the new group section
TODO:
	Integrate with Social Network Graph
		If a Node is linked on the first copy, link it on the New Copy as well
		
*/
function copyDocument($params){
	$DB = new Db();
	$collection = $DB->selectCollection($params["collection"]);
	$OUTPUT = new Output();
	$AUTH = new Auth();
	$LuxFunctions = new LuxFunctions();
	if(isset($params["query"]) || isset($params["id"]) || $LuxFunctions->is_avail("id") || $LuxFunctions->is_avail("query")){
		if($LuxFunctions->is_avail("id") || isset($params["id"])){
			$id = isset($params["id"]) ? $params["id"] : $LuxFunctions->fetch_avail("id");
			$query = array("_id" => new MongoId($id));
		}else{
			$query = $params["query"] ? $params["query"] : $LuxFunctions->fetch_avail("query");
		}
		$cursor = $collection->find($query);
		$printList = array();
		foreach($cursor as $result){
			$old_id = $results["_id"];
			$result["_id"] = new MongoId();
			$collection->insert($result);
			$newDocument = $collection->findAndModify(array("_id" => $results["_id"]), $params["updates"]); 
			if(isset($params["R"]) && $params["R"] != false){
				$params2 = array();
				foreach($params["R"] as $key => $value){
					$params2[$key] = $value;
				}
				if(!isset($params2["query"])){
					$params2["query"] = array("group" => $newDocument["_id"]);
				}
				if(isset($params["R"]["newGroup"]) && $params["R"]["newGroup"] != false){
					if(!isset($params2["updates"])){
						$params2["updates"] = array();	
					}
					$params2["updates"]["group"] = $newDocument["_id"];
				}
				if(isset($params["R"]["print"]) && $params["R"]["print"]){
					$params2["print"] = true;
				}else{
					$params2["print"]  = false;
				}
				copyDocument($params2);
			}
			if(isset($params["enQueue"]) && $params["enQueue"]){
				$DB->enQueue($results["_id"], $AUTH->getClientId(), $newDocument, $LuxFunctions->fetch_avail("priority"));
			}
			$printList[] = $newDocument;
		}
		if(isset($params["print"]) && $params["print"] != false){
			$OUTPUT->success("Document Successfully Copied", array("new Document(s)" => $printList));
		}
	}
}

?>
