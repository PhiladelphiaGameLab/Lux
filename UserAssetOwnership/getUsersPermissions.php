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

$uid = $LF->fetch_avail("uid");


$query = array("_id" => new MongoId($uid)); 
$document = $collection->findOne($query, array("permissions"=>1));
unset($document["lux_info"]);
$OUTPUT->success(0,$document, null);

?>

  
