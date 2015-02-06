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

<<<<<<< HEAD
$results = $collection->find(
    array(
	"userId"=> $AUTH->getClientId()
	)
);

$OUTPUT->success($results);
=======
$userID = $AUTH->getClientId();
$cardID = $LF->fetch_avail('cardID');
$cardScore = $LF->fetch_avail('cardScore');

$query = array(
    'userID' => $userID,
    'metric.cardID' => $cardID
);

$update = array(
    '$set' => array(
        'metric.$.cardScore' => $cardScore
    )
);

$options = array(
    'upsert' => true
);

$results = $collection->update($query, $update, $options);

$OUTPUT->success("success", $results);
>>>>>>> 1466643d15cfc9d08a2ea73fc20b74241890a42e
