<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db("System");
$OUTPUT = new Output();
$LF = new LuxFunctions();
$collection = $db->selectCollection("Users");

$max = $LF->is_avail("max") ? $LF->fetch_avail("max") : 100000;
$lon = $LF->is_avail("lon") ? $LF->fetch_avail("lon") : 0;
$lat = $LF->is_avail("lat") ? $LF->fetch_avail("lat") : 0;

$query = array('loc' => array( '$near' => array('$geometry' => array("type"=>"Point", "coordinates" => array($lon, $lat), '$maxdistance'=>$max))));

$document = $collection->find($query, array("loc"=>true));
//unset($document["lux_info"]);
$OUTPUT->success(0,$document, null);

?>

  
