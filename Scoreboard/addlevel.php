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
$userID = $AUTH->getClientId();

$level = array(
    "_id" => new MongoId(),
    "levelposition" => $LF->fetch_avail("levelposition"),
    "levelID" => $LF->fetch_avail("levelID"),
    "levelname" => $LF->fetch_avail("levelname"),
    "metrics" => array()
);

$query = array(
    "userID" => $LF->fetch_avail("userID"),
);

$update = array(
    '$push' => array("levels" => $level)
);

$results = $collection->update($query, $update);

$OUTPUT->success("success", $results);