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
    "userID" => $LF->fetch_avail("userID"),
);

$response = array("score");

$results = $collection->findOne($query, $response);

$OUTPUT->success("success", $results);  
