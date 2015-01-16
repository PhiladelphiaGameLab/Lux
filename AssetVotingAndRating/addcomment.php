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
    "assets" => array('$in' => $LF->fetch_avail("oldasset"))
);

$update = array(
    '$pull' => array("assets" => $LF->fetch_avail("oldasset")),
    '$push' => array("assets" => $LF->fetch_avail("newasset"))
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