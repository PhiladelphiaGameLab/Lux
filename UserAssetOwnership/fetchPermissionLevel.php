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

$asset = $LF->fetch_avail("asset");


$query = array("_id" => $AUTH->getClientId(), "permissions.$"=>$asset); 
$document = $collection->findOne($query);//, array("permissions.$"=>1));
unset($document["lux_info"]);
$OUTPUT->success(0,$document, null);

?>

  
