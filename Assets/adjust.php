<?php
include_once('./core.php');
$AUTH = new Auth();

// set the options (limited for key->values)
$options = array("new" => true, "upsert"=>true, "multiple"=>true);

// we got a query or an id
// If an id is used instead of a query, then a query must be built
if($LF->is_avail("id")){
	$query = array("_id" => new MongoId($LF->fetch_avail("id")));
}else if($LF->is_avail("query")){
	$query = $LF->fetch_avail("query");
	if(!is_array($query)){
		$query = array($query);
	}
}else{
	$OUTPUT->error(0, "Query or Id is required");
}
$documents = $collection->find($query);
foreach($documents as $key=>$value){
	UserAssetOwnership::check($key, $collectionName, 1);
}
$update = array('$set' => array($LF->fetch_avail("key") => $LF->fetch_avail("value")));
$results = $collection->update($query, $update, $options);
$documents = $collection->find($query);
$OUTPUT->success(0, $documents, $results);

/*
if(!$LF->is_avail("noPub") || $LF->fetch_avail("noPub")){
	foreach($documents as $document){
		if(isset($removed) && $removed){
			$document["removed"] = true;
		}
		$db->publish($document, $AUTH); 
	}
}
*/
die();
  
