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

$_id = new MongoId();

if ($LF->is_avail('parent_id')) {
    $parent_id = $LF->fetch_avail('parent_id');
    $parent_array = $LF->fetch_avail('parent_array');
    array_push($parent_array, $parent_id);
} else {
    $parent_id = null;
    $parent_array = array();
    $collection->update(
        $query = array(
            '_id' => $LF->fetch_avail('asset_id')
        ),
        $update = array(
            '$addToSet' => array(
                'comment_ids' => $_id
            )
        )
    );

}

$comment = array(
    '_id' => $_id,
    'user_id' => $AUTH->getClientId(),
    'comment_body' => $LF->fetch_avail('comment_body'),
    'parent_id' => $parent_id,
    'asset_id' => $LF->fetch_avail('asset_id'),
    'date' => new DateTime('now'),
    'ratings' => array(),
    'upvotes' => array(),
    'downvotes' => array(),
    'parent_array' => $parent_array
);

$results = $collection->insert($comment);

$OUTPUT->success("comment successfully added", $results);  
