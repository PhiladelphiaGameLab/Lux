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
    "_id" => $userID,
);

$update = array(
    '$set' => array("score".$LF->fetch_avail("type") => $LF->fetch_avail("newscore"))
);

$results = $collection->findAndModify(
    $query,
    $update,
    null,
    array(
        "new" => true
    )
);

$OUTPUT->success("success", $results);
  
