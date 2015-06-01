<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db("System");
$OUTPUT = new Output();
$collection = $db->selectCollection("Users");
$AUTH = new Auth();
$LF = new LuxFunctions();

$level = $LF->fetch_avail("level");
if($level > 4){
	$OUTPUT->error(2, "Level is out of bounds");
}

$assets = $LF->fetch_avail("asset");
if(is_array($assets)){
	$OUTPUT->error(2, "Assets Ownership can only be updated one asset per call");
}
$operation = '$addToSet';
if($LF->is_avail("revoke") && $LF->fetch_avail("revoke")){
	$assets = array($assets);
	$operation = '$pullAll';
}

// Adjust the User's document
$update = array($operation => array("permissions.".$level => $assets));

$options["upsert"] = true;
$options["multiple"] = false;
$options["new"] = true;

$query = array("_id" => $AUTH->getClientId()); 
$results = $collection->update($query, $update, $options);
$document = $collection->findOne($query);
unset($document["lux_info"]);
$OUTPUT->success(0,$document, $results);

// Adjust the Asset Document
if($level == 2 || $level == 3){
	$db = new Db("Assets");
	$collections = $db->getCollectionNames();
	foreach ($collections as $collectionName) {
		$collection = $db->selectCollection($collectionName);
		$update = array('$set' => array("permissions.".$level => 0));
		$options["upsert"] = true;
		$options["multiple"] = false;
		$options["new"] = true;
		$query = array("_id" => new MongoId($assets)); 
		$results = $collection->update($query, $update, $options);
	}
}
?>

  
