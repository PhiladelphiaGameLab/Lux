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
    '$set' => array("levels".$LF->fetch_avail('levelID')."metrics".$LF->fetch_avail("metricarray") => $LF->fetch_avail("metric"))
);

$results = $collection->update($query, $update);

$OUTPUT->success("success", $results);
  
