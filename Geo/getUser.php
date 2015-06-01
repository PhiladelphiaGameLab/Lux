<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db("System");
$OUTPUT = new Output();
$LF = new LuxFunctions();
$collection = $db->selectCollection("Users");

$query = array("_id" => $LF->fetch_avail("id"));

$document = $collection->find($query, array("loc.coordinates"=>true));
//unset($document["lux_info"]);
$OUTPUT->success(0,$document, null);

?>

  
