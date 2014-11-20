<?php
// seems functionally complete
include_once("lux-functions.php");
include_once("output.php");
include_once("db.php");

class Db{

	private $db;
	private $OUTPUT;

	function __construct(){
		$this->OUTPUT = new Output();
		if(class_exists("MongoClient")){
			$MON = new MongoClient("mongodb://localhost/");
		}else{
			$this->OUTPUT->error("Lux is not Properly Set-up, can not find MongoClient");
		}
		$this->db = $MON->selectDB("Lux2");
	}

	function selectCollection($collectionName){
		return $this->db->selectCollection($collectionName);
	}

	function enQueue($documentId, $senderId, $document, $priority){
		$priority = $priority? $priority : "0";
		$doc=array("sender" => $senderId, "timestamp" => microtime(), "priority"=> $priority, "data" => $document, "checked_by" => array("python" => false, "node" => false));
		$options = array( "upsert" => true);
		$Queue = $db->selectCollection("Queue");
		$Queue->update(array('_id' => new MongoId($id)), $doc, $options);
	}

	function PubSub($query, $senderId){
		// add to PubSub
	}

	function findAsset($id){
		$AssetCollection = $this->selectCollection("Assets");
		return $AssetCollection->findOne(array('_id' => new MongoId($id)));
	}	
}


?>




