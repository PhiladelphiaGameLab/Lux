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

$query = array(
    "_id" => new MongoId(),
    "user_id" => $LF->fetch_avail("user_id"), // must be unique
    "username" => $LF->fetch_avail("username"),
    "score" => array(
        "raw" => 0
    ),
    "levels" => array(),
    "assets" => array()
);

$results = $collection->insert($query);

$OUTPUT->success("success", $results);
