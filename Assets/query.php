<?php
include_once('./core.php');
// includes the Auth()
if($LF->is_avail("access_token")){
	$AUTH = new Auth();
	$auth = true;
}

if($LF->is_avail("id")){
	UserAssetOwnership::check($LF->fetch_avail("id"), $collectionName, 1);
	$query = array("_id" => new MongoId($LF->fetch_avail("id")));
	$document = $collection->findOne($query);
	$OUTPUT->success(1, $document);
}else if($LF->is_avail("query")){
	$query = is_array($LF->fetch_avail("query"))? $LF->fetch_avail("query") : array($LF->fetch_avail("query"));
	if($LF->is_avail("distinct") && $LF->fetch_avail("distinct")){
		$documents = $collection->distinct($query);
		foreach($documents as $key => $value){
			UserAssetOwnership::check($key, $collectionName, 1);
		}
		$OUTPUT->success(1, $documents);
	}else if($LF->is_avail("aggregate") && $LF->fetch_avail("aggregate")){
		$documents = $collection->aggregateCursor($query);
		foreach($documents as $key => $value){
			UserAssetOwnership::check($key, $collectionName, 1);
		}
		$OUTPUT->success(1, $documents);
	}else{
		$documents = $collection->find($query, array("lux_info"=>0));
		foreach($documents as $key => $value){
			UserAssetOwnership::check($key, $collectionName, 1);
		}
		$OUTPUT->success(1, array(), $documents);
	}
}
/*
if(!$LF->is_avail("noSub") || $LF->fetch_avail("noSub")){
	$db->subscribe($query, $AUTH);
}
*/
die();

?>

  
