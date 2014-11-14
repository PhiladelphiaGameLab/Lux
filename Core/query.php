<?php
include_once("lux-functions.php");
include_once("output.php");
include_once("db.php");

/*
Params in: 
	$params = array(
		collectionName
		distinct
		aggregate
		pubsub
		enqueue
		resolve
	)

        
Ouput: 
        Outputs an array of documents
Functionality: 
	Queries collection with passed query string
        
TODO:
	uses is_avail instead of isset
	Push into the pub/sub section
	Format on a single parameters matching a description
	Accept $gt, $lt, $rad, $box, and $btw operators
	Specific Limit and Ordering Filters
	Queries can be Filtered by something	
	//Allow for Aggregation as well
	//Queries for Distinct
	//Automatically Resolve DBRefs
	Use Grouping where availiable
*/
function query($params){
	$db = new Db();
        $OUTPUT = new Output();
        $coll = $db->selectCollection($params["collectionName"]);
        $PubSub = $db->selectCollection("PubSub");
	$AUTH = new Auth();
	$LuxFunctions = new LuxFunctions();
	if($LuxFunctions->is_avail("id")){
		$query = array("_id" => new MongoID($LuxFunctions->fetch_avail("id")));
		$result = $coll->findOne($query);
	
	}else if($LuxFunctions->is_avail("query")){
		$query = $LuxFunctions->fetch_avail("query");
		// change to is_avail	
		if(isset($params["distinct"]) && $params["distinct"]){
                	$result = $coll->distinct($query);
	
		}else if(isset($params["aggregate"]) && $params["aggregate"]){
                	$result = $coll->aggregate($query);
		}
	}
	
	if(isset($params["pubsub"]) && $params["pubsub"] == true){
		$check = $PubSub->findOne($query);
	
		if(isset($check)){
			$PubSub->update(array("_id" => $check["id"]) , array('$addToSet' => array("subscribers" => array("clientId" => $AUTH->getClientId(), "subscribetime" => microtime()))));
			$PubSub->upsert(array("_id" => $check["id"]), array("timestamp" => microtime()));
		}else{
			$newdoc = array("query" => $query, "subscribers" => array("clientId" => $AUTH->getClientId(), "subscribetime" => microtime()), "timestamp" => microtime(), "parent-sub" => null);
			$PubSub->insert($newdoc);
		}
	}

	if(isset($params["enqueue"]) && $params["enqueue"] == true){
		 $db->enQueue($LuxFunctions->fetch_avail("id"), $AUTH, $LuxFunctions->fetch_avail("doc", false), $params["priority"]);	
	}
	
	if(isset($params["resolve"]) && $params["resolve"]){
		// loop through the documents and resolve the dbRefs
	}

	
}

?>
