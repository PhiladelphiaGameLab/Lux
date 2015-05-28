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

$permitted = array("email.primary", "email.other[]", "name.given", "name.family", "pictures.primary", "pictures.other[]", "gender", "locale", "domain", "name.full", );

$update = array();
foreach($LF->getParameters() as $key => $value){
	if(in_array($key, $permitted)){
		if(substr($key, -2) == "[]"){
			// this needs to be pushed into the array
			$update['$addToSet']["values.".substr($key, 0, -2)] = $value;
		}else{
			// this can be set manually
			$update['$set']["values.".$key] = $value;
		}
	}
}
$options["upsert"] = true;
$options["multiple"] = false;
$options["new"] = true;
$query = array("_id" => $AUTH->getClientId()); 
$results = $collection->update($query, $update, $options);
$document = $collection->findOne($query);
unset($document["lux_info"]);
$OUTPUT->success(0,$document, $results);

?>

