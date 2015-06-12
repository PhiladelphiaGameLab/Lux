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
    '_id' => $Lf->fetch_avail('object_id')
);

$update = array(
    '$pull' => array(
        'downvotes' => $AUTH->getClientId()
    )
);

$results = $collection->update($query, $update);

$OUTPUT->success("downvote successfully removed", $results);  
