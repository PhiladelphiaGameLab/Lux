<?php
/*
Params in:
        doc: the full or partial document update
        id || query: the document _id or the Query
        access_token: the user's access token
Ouput: 
        updated document(s)
Functionality:
        Takes a full document, or a document update, and either creates a new document or updates an existing document in the Queue

*/

include("../core/upsert.php");
include("../core/Auth.php");
include("../core/lux-functions.php");

$LF = new LuxFunctions();
$Auth = new Auth();
$params = array();
$params["collectionName"] = "Queue";
$params["doc"] = is_avail("doc", false);
$params["_id"] = is_avail("id", false, $params["doc"]);
$params["auth"] = $Auth;
$params["query"] = is_avail("query", false);
upsert($params);
?>

