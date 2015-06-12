<?php

include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db();
$OUTPUT = new Output();
$collection = $db->selectCollection('Scoreboard');
$LF = new LuxFunctions();
$AUTH = new Auth();

$query = array(
    'userID' => $AUTH->getClientId(),
);

$update = array(
    '$set' => array('assets'.$LF->fetch_avail('asset') => $LF->fetch_avail('quantity')),
);

$results = $collection->update(
    $query,
    $update,
    array(
        'upsert' => true
    )
);

$OUTPUT->success('Asset successfully adjusted', $results);  
