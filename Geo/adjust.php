<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db("System");
$OUTPUT = new Output();
$LF = new LuxFunctions();
$collection = $db->selectCollection("Users");
$AUTH = new Auth();

$lon = $LF->is_avail("lon") ? $LF->fetch_avail("lon") : 0;
$lat = $LF->is_avail("lat") ? $LF->fetch_avail("lat") : 0;

$update = array('$set' => array( "loc" => array("type"=>"Point", "coordinates" => array($lon, $lat))));

$options["upsert"] = true;
$options["multiple"] = false;
$options["new"] = true;
$query = array("_id" => $AUTH->getClientId());
$results = $collection->update($query, $update, $options);
$document = $collection->findOne($query, array("loc"=>true));
unset($document["lux_info"]);
$collection->ensureIndex(array('loc' => '2dsphere'));
$OUTPUT->success(0,$document, $results);


?>

  
