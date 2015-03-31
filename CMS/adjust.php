<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db("System");
$OUTPUT = new Output();
$collection = $db->selectCollection("CMS");
$AUTH = new Auth();
if(!$AUTH->isAdmin("CMS")){
	$OUTPUT->error(3, "User does not have permission to edit content");
}
$LF = new LuxFunctions();

$permitted = array("content.full", "content.short", "content.medium", "header.text","header.sub", "header.url_safe", "picture.banner", "picture.other[]", "picture.slideshow[]");

$update = array();
foreach($LF->getParameters() as $key => $value){
	if(in_array($key, $permitted)){
		if(substr($key, -2) == "[]"){
			// this needs to be pushed into the array
			$update['$addToSet'][substr($key, 0, -2)] = $value;
		}else{
			// this can be set manually
			$update['$set'][$key] = $value;
		}
	}
}
$options["upsert"] = true;
$options["multiple"] = false;
$options["new"] = true;
$query = array("field" => $LF->fetch_avail("field")); 
$results = $collection->update($query, $update, $options);
$document = $collection->findOne($query);
unset($document["lux_info"]);
$OUTPUT->success(0,$document, $results);

?>

