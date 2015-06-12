<?php
include_once('./core.php');
$AUTH = new Auth();

// Set the options here
if($LF->is_avail("options")){
	$options = $LF->fetch_avail("options");
	$options["new"] = true;
}else{
	$options = array("new" => true);
}

// an update is available
if($LF->is_avail("update")){
	$update = $LF->fetch_avail("update");
}
// we got a query or an id
// If an id is used instead of a query, then a query must be built
if($LF->is_avail("id")){
	UserAssetOwnership::check($LF->fetch_avail("id"), $collectionName, 1);
	$query = array("_id" => new MongoId($LF->fetch_avail("id")));
}else if($LF->is_avail("query")){
	$query = $LF->fetch_avail("query");
	if(!is_array($query)){
		$query = array($query);
	}
}

if(isset($update) && isset($query)){
	// upsert
	$options["upsert"] = true;
	if(isset($update['$set'])){
		$options["multiple"] = true;
	}else{
		$update["permissions"]["3"] = 1;
		$update["permissions"]["2"] = 1;
	}
	$documents = $collection->find($query);
	foreach($documents as $key=>$value){
		UserAssetOwnership::check($key, $collectionName, 1);
	}
	$results = $collection->update($query, $update, $options);
	$documents = $collection->find($query);
	$collection->update($query, array('$set'=>array('timestamp'=>microtime())));
	$OUTPUT->success(0,$documents, $results);
}else if(isset($query) && !isset($update)){
	// remove
	$documents = $collection->find($query);
	foreach($documents as $key=>$value){
		UserAssetOwnership::check($key, $collectionName, 1);
	}
	$results = $collection->remove($query);
	$removed = true;
	$OUTPUT->success(2, $documents, $results);
}else if(isset($update) && !isset($query)){
	// insert
	$update["timestamp"] = microtime();
	$update["permissions"]["3"] = 1;
	$update["permissions"]["2"] = 1;
	$results = $collection->insert($update);
	$documents = $collection->find($update); 
	$OUTPUT->success(0, $documents, $results);
}else{
	// missing parameters
	$LF->fetch_avail("update");
	$LF->fetch_avail("query");
	$LF->fetch_avail("id");
}
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
  
