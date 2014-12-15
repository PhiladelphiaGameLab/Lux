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

if($LF->is_avail("id")){
        $query = array("_id" => new MongoId($LF->fetch_avail("id")));
        $document = $collection->findOne($query);
        $OUTPUT->success("Found User", $document);
}else if($LF->is_avail("query")){
        $query = is_array($LF->fetch_avail("query"))? $LF->fetch_avail("query") : array($LF->fetch_avail("query"));
        $documents = $collection->find($query);
        $OUTPUT->success("Found Users", $documents);
}else if($LF->is_avail("distinct")){
        $documents = $collection->distinct($LF->fetch_avail("distinct"));
        $OUTPUT->success("Found Distinct ".$LF->fetch_avail("distinct")."s", $documents);
}else if($LF->is_avail("aggregate")){
        $documents = $collection->aggregateCursor($LF->fetch_avail("aggregate"));
        $OUTPUT->success("Found Aggregate Values", $documents);
}else{
        $query = array("_id" => new MongoId($AUTH->getClientId()));
        $document = $collection->findOne($query);
        $OUTPUT->success("Found User", $document);
}
$db->subscribe($query, $AUTH);


?>

