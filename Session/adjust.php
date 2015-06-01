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

$keys = $LF->fetch_avail("key");
$values = $LF->fetch_avail("value");

$update['$set'] = array();
if(is_array($keys) && is_array($values) && count($keys) == count($values)){
	// treat them as arrays
	foreach($keys as $index => $key){
		// iterate and create a set variable
		$update['$set'][$key] = $values[$index];
		
		// iterate and create the session variable
		$_SESSION[$key] = $values[$index];
	}
}else if(!is_array($keys)){
	// just treat them as a single value
	$_SESSION[$keys] = $values;
	$update['$set'] = array($keys => $values);

}else{
	$OUTPUT->error(2, "Key array must be same length as Value array");
}




$options["upsert"] = true;
$options["multiple"] = false;
$options["new"] = true;
$query = array("_id" => $id); 
$results = $collection->update($query, $update, $options);
$document = $collection->findOne($query);
unset($document["lux_info"]);
$documents = array($document, $_SESSION);
$OUTPUT->success(0,$documents, $results);

?>

  
