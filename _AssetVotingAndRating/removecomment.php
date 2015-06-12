<?php

include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db();
$OUTPUT = new Output();
$collection = $db->selectCollection("AssetVotingAndRating");
$LF = new LuxFunctions();
$AUTH = new Auth();

$query = array(
    '_id' => $LF->fetch_avail('comment_id'),
    'user_id' => $AUTH->getClientId()
);

$update = array(
    '$set' => array(
        'comment_body' => '[deleted]'
    )
);

$results = $collection->update($query, $update);

$OUTPUT->success("comment successfully deleted", $results);  
