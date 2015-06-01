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
*/
function query($params){
	$db = new Db();
        $OUTPUT = new Output();
        $coll = $db->selectCollection($params["collectionName"]);
        $Subscribe = $db->selectCollection("Subscribers");
	$AUTH = new Auth();
	$LuxFunctions = new LuxFunctions();
	$LuxFunctions->setDocument($params);
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
		}else{
			$result = $coll->find($query);
		}
	}
	echo json_encode($result);
	if(isset($params["subscribe"]) && $params["subscribe"] == true){
		$key = array_keys($query);
		$value = array_values($query); 
		$cursor = $Subscribe->findOne(array("query.".$key[0] => $value[0]));		
		if(isset($cursor)){
			$Subscribe->update(array("_id" => $cursor["_id"]) , array('$addToSet' => array("subscribers.clientId" => $AUTH->getClientId())));
		}else{
			$newdoc = array("query" => $query, "subscribers" => array("clientId" => array($AUTH->getClientId())), "timestamp" => microtime(), "parent_sub" => null);
			$Subscribe->insert($newdoc);
		}
	}
	if(isset($params["publish"]) && $params["publish"] == true){
		 $db->Publish($LuxFunctions->fetch_avail("doc", false), $AUTH, $params["priority"]);	
	}
}

?>
  
