<?php
include_once("lux-functions.php");
include_once("output.php");
include_once("db.php");
include_once("auth.php");

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
	$LuxFunctions->setDocument($params);
	if($LuxFunctions->is_avail("id")){
		$query = array("_id" => new MongoID($LuxFunctions->fetch_avail("id")));
		$result = $coll->findOne($query);
	
	}else if($LuxFunctions->is_avail("query")){
		$query = $LuxFunctions->fetch_avail("query");
		var_dump($query);
		// change to is_avail	
		if(isset($params["distinct"]) && $params["distinct"]){
                	$result = $coll->distinct($query);
	
		}else if(isset($params["aggregate"]) && $params["aggregate"]){
                	$result = $coll->aggregate($query);
		}
	}
	
	if(isset($params["pubsub"]) && $params["pubsub"] == true){
		$key = array_keys($query);
		$value = array_values($query); 
		$cursor = $PubSub->findOne(array("query.".$key[0] => $value[0]));		var_dump($cursor);
		if(isset($cursor)){
			$PubSub->update(array("_id" => $cursor["_id"]) , array('$addToSet' => array("subscribers.clientId" => $AUTH->getClientId())));
			//$PubSub->upsert(array("_id" => $check["id"]), array("timestamp" => microtime()));
		}else{
			echo"new doc!";
			$newdoc = array("query" => $query, "subscribers" => array("clientId" => array($AUTH->getClientId())), "timestamp" => microtime(), "parent_sub" => null);
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
