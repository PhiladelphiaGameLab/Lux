<?php
// This is also probably done
require('../core/Auth.php');
require('../core/db.php');
require('../core/output.php');
require('../core/lux-functions.php');
/*
Params in:
	_id of document (optional)
	document 		
	priority (optional)
Ouput: 
	updated document
Functionality:
	Takes a full document, or a document update, and either creates a new document or updates an existing document.
	if No _id is provided, new document is always created.	
*/

$AssetCollection = $DB->selectCollection("Assets");
$client_id = $AUTH->getClientId(); // This is a mongo id 
$priority = is_avail("priority", false)? is_avail("priority") : "Low";


// check for variable existance
$document = is_post("data");
$id = is_avail("id", false, $document); // in lux-functions.php
//$id = is_get("id");
// write to Asset Database with Appropriate Query
if($id){
	$AssetCollection->update(array('_id' => new MongoId($id)), $document, array( "upsert" => true);
}else{
	$AssetCollection->insert($document);
}

// Add update to Mongo Queue
$DB->enQueue($id, $client_id, $document, $priority); // in lux-functions

// print output 
if($id){
	$updatedDocument = $AssetCollection->findOne(array('_id' => new MongoId($id)));
	$OUTPUT->success("Document was Successfully updated", $updatedDocument);
}else{
	$OUTPUT->success("Document was Successfully updated", $document);
}
?>
