<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db("System");
$OUTPUT = new Output();
$collection = $db->selectCollection("Session");
$LF = new LuxFunctions();

// get the session id if the front-end knows it
if($LF->is_avail("session_id")){
	$id = $LF->fetch_avail("session_id");
}

// try to create a session with that id:
if(isset($id)){
	$id = session_id($id);
}
session_start();

// set id to any session id that may have been found
$id = session_id();
$_SESSION["session_id"] = $id;

$query = array("_id" => $id); 
$document = $collection->findOne($query);
unset($document["lux_info"]);
$documents = array($document, $_SESSION);
$OUTPUT->success(0,$documents, null);

?>

  
