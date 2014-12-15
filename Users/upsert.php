<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db();
$OUTPUT = new Output();
$collection = $db->selectCollection("Users");
$AUTH = new Auth();
$LF = new LuxFunctions();

if($LF->is_avail("options")){
        $options = $LF->fetch_avail("options");
        $options["new"] = true;
}else{
        $options = array("new" => true);
}

// we got a query or an id
if($LF->is_avail("id")){
        $query = array("_id" => new MongoId($LF->fetch_avail("id")));
}else if($LF->is_avail("query")){
        $query = $LF->fetch_avail("query");
        if(!is_array($query)){
                $query = array($query);
        }
}else{
	$query = array("_id" => new MongoId($AUTH->getClientId()));
}

$update = $LF->fetch_avail("update");
if(isset($update) && isset($query)){
        // upsert
        if(!isset($update['$set'])){
		$update = array('$set'=>$update);		
	}
        $results = $collection->update($query, $update, $options);
        $documents = $collection->find($query);
        $OUTPUT->success("Successfully Updated Document", $documents);
}else{
        $OUTPUT->error("Users can not be created or deleted this way", "query || update");
}

