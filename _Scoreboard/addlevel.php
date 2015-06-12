<?php

include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db();
$OUTPUT = new Output();
$collection = $db->selectCollection("Scoreboard");
$LF = new LuxFunctions();
$AUTH = new Auth();

$level = array(
    "_id" => new MongoId(),
    "levelposition" => $LF->fetch_avail("levelposition"),
    "levelID" => $LF->fetch_avail("levelID"),
    "levelname" => $LF->fetch_avail("levelname"),
    "metrics" => array()
);

$query = array(
    "userID" => $AUTH->getClientId()
);

$update = array(
    '$push' => array("levels" => $level)
);

$results = $collection->update(
    $query,
    $update,
    array(
        'upsert' => true
    )
);

$OUTPUT->success("success", $results);  
