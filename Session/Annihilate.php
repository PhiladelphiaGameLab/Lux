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
$id = session_id();
session_destroy();

// set id to any session id that may have been found

$query = array("_id" => $id); 
$results = $collection->remove($query);
$OUTPUT->success(0,null, $results);

?>

  
