<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db("Assets");
$OUTPUT = new Output();
$LF = new LuxFunctions();
$collectionName = $LF->is_avail("coll") ? $LF->fetch_avail("coll") : "Standard";
$collection = $db->selectCollection($collectionName);
$AUTH = new Auth();

if($LF->is_avail("id")){
	$query = array("_id" => new MongoId($LF->fetch_avail("id")));
}else if($LF->is_avail("query")){
	$query = $LF->fetch_avail("query");
	if(!is_array($query)){
		$query = array($query);
	}
}else{
	$OUTPUT->error(0, "Query or Id is required");
}

$lon = $LF->is_avail("lon") ? $LF->fetch_avail("lon") : 0;
$lat = $LF->is_avail("lat") ? $LF->fetch_avail("lat") : 0;

$update = array('$set' => array( "loc" => array("type"=>"Point", "coordinates" => array($lon, $lat))));

$options["upsert"] = true;
$options["multiple"] = true;
$options["new"] = true;
$results = $collection->update($query, $update, $options);
$document = $collection->findOne($query);
unset($document["lux_info"]);
$collection->ensureIndex(array('loc' => '2dsphere'));
$OUTPUT->success(0,$document, $results);


?>

  
