<?php
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Headers: Content-Type, Content-Range, Content-Disposition, Content-Description");
// seems functionally complete
include_once("lux-functions.php");
include_once("output.php");
include_once("db.php");

class Db{

	private $db;
	private $OUTPUT;

	function __construct($db){
		$this->OUTPUT = new Output();
		if(class_exists("MongoClient")){
			$MON = new MongoClient("mongodb://localhost/");
		}else{
			$this->OUTPUT->error("Lux is not Properly Set-up, can not find MongoClient");
		}
		if($db == null){
			$this->db = $MON->selectDB("System");
		}else{
			$this->db = $MON->selectDB($db);
		}
	}

	function selectCollection($collectionName){
		return $this->db->selectCollection($collectionName);
	}

	function publish($document, $AUTH, $priority=0){
		$document["info"] = array("sender" => $AUTH->getClientId(), "timestamp" => microtime(), "priority"=> $priority, "checked_by" => array("python" => false, "node" => false));
		$Published = $this->selectCollection("Published");
		$Published->update(array("_id"=>$document["_id"]),$document, array("upsert"=>true));
	}
	
	function unpublish($document, $AUTH, $priority=0){
		$Published = $this->selectCollection("Published");
		$Published->remove(array("_id"=>$document["_id"]));
	}

	function subscribe($query, $AUTH){
		$subscribers = $this->selectCollection("Subscribers");
		$subDoc = $subscribers->update(
		array("query"=>$query)
		,array('$addToSet'=> array("subscribers" => array("id" => $AUTH->getClientId())) 
	        ,'$set'=>array("timestamp"=>microtime()))
		,array("upsert"=>true));
	}
	
	function unsubscribe($query, $AUTH){
		$subscribers = $this->selectCollection("Subscribers");
		$subDoc = $subscribers->update(
		array("query"=>$query)
		,array('$pull'=> array("subscribers" => array(array("id"=>$AUTH->getClientId()))) 
	        ,'$set'=>array("timestamp"=>microtime()))
		,array("upsert"=>true));
	}

	function findAsset($id){
		$AssetCollection = $this->selectCollection("Assets");
		return $AssetCollection->findOne(array('_id' => new MongoId($id)));
	}	
}


?>




