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

$query = array("_id" => $AUTH->getClientId()); 
$document = $collection->findOne($query, array("permissions.".$level=>1, "_id"=>0));
unset($document["lux_info"]);
$OUTPUT->success(0,$document, null);

?>

  
