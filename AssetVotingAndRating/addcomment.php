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

if ($LF->fetch_avail('comment_type') == 'reply') {
    $parent_id = $LF->fetch_avail('parent_id');
} else {
    $parent_id = null;
}

$comment = array(
    'user_id' => $AUTH->getClientId(),
    'comment_body' => $LF->fetch_avail('comment_body'),
    'parent_id' => $parent_id,
    'asset_id' => $LF->fetch_avail('asset_id'),
    'date' => new DateTime('now'),
    'ratings' => array(),
    'upvotes' => array(),
    'downvotes' => array()
);

$results = $collection->insert($comment);

$OUTPUT->success("comment successfully added", $results);