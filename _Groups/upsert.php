<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db();
$OUTPUT = new Output();
$collection = $db->selectCollection("Groups");
$AUTH = new Auth();
$LF = new LuxFunctions();

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
if($LF->is_avail("id")){
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
        }
        $results = $collection->update($query, $update, $options);
        $documents = $collection->find($query);
        $OUTPUT->success("Successfully Updated Group", $results);
}else if(isset($query) && !isset($update)){
        // remove
        $documents = $collection->find($query);
        $results = $collection->remove($query);
        $removed = true;
///////// remove Social Network Node

        $OUTPUT->success("Successfully Removed Group", $results);
}else if(isset($update) && !isset($query)){
        // insert
///////// create Node in Social Network Graph

        $results = $collection->insert($update);
        $documents = $collection->find($update);
        $OUTPUT->success("Successfully Inserted Document", $results);
}else{
        $OUTPUT->error("Parameters are missing", "query || update");
}
foreach($documents as $document){
        if(isset($removed) && $removed){
                $document["removed"] = true;
        }
	//$db->publish($document, $AUTH);
}


?>

  
