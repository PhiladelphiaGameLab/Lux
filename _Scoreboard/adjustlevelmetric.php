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

$query = array(
    "userID" => $AUTH->getClientId(),
    "levels.levelID" => $LF->fetch_avail("levelID")
);

$update = array(
    '$set' => array(
        $LF->fetch_avail("levelmetric") => $LF->fetch_avail("levelposition"))
);

$options = array(
    "upsert" => true
);

$results = $collection->update(
    $query,
    $update,
    $options
);

$OUTPUT->success("success", $results);  
