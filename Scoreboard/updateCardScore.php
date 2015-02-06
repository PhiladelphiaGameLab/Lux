<?php
<<<<<<< HEAD
=======

>>>>>>> 1466643d15cfc9d08a2ea73fc20b74241890a42e
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');
<<<<<<< HEAD
=======

>>>>>>> 1466643d15cfc9d08a2ea73fc20b74241890a42e
$db = new Db();
$OUTPUT = new Output();
$collection = $db->selectCollection("Scoreboard");
$LF = new LuxFunctions();
$AUTH = new Auth();
<<<<<<< HEAD
$userID = $AUTH->getClientId();
$cardID = $LF->fetch_avail('cardID');
$cardScore = $LF->fetch_avail('cardScore');
=======

$userID = $AUTH->getClientId();
$cardID = $LF->fetch_avail('cardID');
$cardScore = $LF->fetch_avail('cardScore');

>>>>>>> 1466643d15cfc9d08a2ea73fc20b74241890a42e
$query = array(
    'userID' => $userID,
    'metric.cardID' => $cardID
);
<<<<<<< HEAD
=======

>>>>>>> 1466643d15cfc9d08a2ea73fc20b74241890a42e
$update = array(
    '$set' => array(
        'metric.$.cardScore' => $cardScore
    )
);
<<<<<<< HEAD
$options = array(
    'upsert' => true
);
$results = $collection->update($query, $update, $options);
$OUTPUT->success("success test 2.4.15", $results);
=======

$options = array(
    'upsert' => true
);

$results = $collection->update($query, $update, $options);

$OUTPUT->success("success test 2.4.15", $results);
>>>>>>> 1466643d15cfc9d08a2ea73fc20b74241890a42e
