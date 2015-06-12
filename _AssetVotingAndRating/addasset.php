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

$asset = array(
    'user_id' => $AUTH->getClientId(),
    'source' => $LF->fetch_avail('source'),
    'date' => new DateTime('now'),
    'comment_ids' => array(),
    'ratings' => array(),
    'upvotes' => array(),
    'downvotes' => array()
);

$results = $collection->insert($asset);

$OUTPUT->success("asset successfully added", $results);  
