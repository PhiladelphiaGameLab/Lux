<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db("System");
$OUTPUT = new Output();
$collection = $db->selectCollection("Contact");
$AUTH = new Auth();
if(!$AUTH->isAdmin("Contact")){
	$OUTPUT->error(3, "User does not have permission to edit content");
}
$LF = new LuxFunctions();

$email_address = $LF->fetch_avail("email_address");
if($LF->is_avail("remove") && $LF->fetch_avail("remove")){
	$update = array('$pull' => array("addresses" => $email_address)); 
}else{
	$update = array('$addToSet' => array("addresses" => $email_address)); 
}

$options["upsert"] = true;
$options["multiple"] = false;
$options["new"] = true;
$query = array("email_id" => $LF->fetch_avail("email_id")); 
$results = $collection->update($query, $update, $options);
$document = $collection->findOne($query);
unset($document["lux_info"]);
$OUTPUT->success(0,$document, $results);

?>

  
