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
    "userID" => $AUTH->getClientId()
);

$metricarray = array(
    $LF->fetch_avail('metricname') => $LF->fetch_avail('metric')
);

$update = array(
    '$push' => array("levels".$LF->fetch_avail('levelID')."metrics" => $metricarray)
);

$results = $collection->update(
    $query,
    $update,
    array(
        'upsert' => true
    )
);

$OUTPUT->success("success", $results);  
