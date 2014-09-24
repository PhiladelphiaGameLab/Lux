<?php
/*
Params in:
        doc: the full or partial document update
        id || query: the document _id of the orignal document
        access_token: the user's access token
Ouput: 
        updated document(s)
Functionality:
        Takes a full document, or a document update, and either creates a new document or updates an existing document in the Asset Database

*/

include("../core/upsert.php");
include("../core/Auth.php");
include("../core/lux-functions.php");

$LF = new LuxFunctions();
$Auth = new Auth();
$params = array();
$updates = is_avail("doc", false);


if(is_avail("id", false){
        $params["query"] = is_avail("query", false);
        $params["_id"] = is_avail("id");
}else{
        $params["_id"] = is_avail("id", false);
        $params["query"] = is_avail("query");
}
copyOne($id, $updates, "Assets");
?>

