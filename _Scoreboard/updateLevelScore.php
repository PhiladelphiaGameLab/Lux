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
$levelID = $LF->fetch_avail('levelID');
$levelScore = $LF->fetch_avail('levelScore');

$query = array(
    'userID' => $userID,
    'level.levelID' => $levelID
);
$update = array(
    '$set' => array(
        'levels.levelScore' => $levelScore
    )
);
$options = array(
    'upsert' => true
);

$results = $collection->update($query, $update, $options);

$OUTPUT->success("success", $results);
  
