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
$levelID = $LF->fetch_avail('levelID');
$levelScore = $LF->fetch_avail('levelScore');
=======

$userID = $AUTH->getClientId();
$levelID = $LF->fetch_avail('levelID');
$levelScore = $LF->fetch_avail('levelScore');

>>>>>>> 1466643d15cfc9d08a2ea73fc20b74241890a42e
$query = array(
    'userID' => $userID,
    'level.levelID' => $levelID
);
<<<<<<< HEAD
=======

>>>>>>> 1466643d15cfc9d08a2ea73fc20b74241890a42e
$update = array(
    '$set' => array(
        'levels.$.levelScore' => $levelScore
    )
);
<<<<<<< HEAD
$options = array(
    'upsert' => true
);
$results = $collection->update($query, $update, $options);
$OUTPUT->success("success", $results);
=======

$options = array(
    'upsert' => true
);

$results = $collection->update($query, $update, $options);

$OUTPUT->success("success", $results);
>>>>>>> 1466643d15cfc9d08a2ea73fc20b74241890a42e
