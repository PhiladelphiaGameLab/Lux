<?php

/*
 *
 * Ajax(
 * ""whereever you put this file,
 * {'leftscore': some input, 'rightscore': some input}
 *
 * ),
 *
 */

include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db();
$OUTPUT = new Output();
$collection = $db->selectCollection("Pong");
$LF = new LuxFunctions();
$AUTH = new Auth();

$level = array(
		'userID' => $AUTH->getClientId(),
        'left' => $LF->fetch_avail('leftscore'),
        'right' => $LF->fetch_avail('rightscore'),
);

$results = $collection->insert($level);

$OUTPUT->success("success", $results);
