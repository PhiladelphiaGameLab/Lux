<?php

include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');
include_once('../Core/resolve.php');

$db = new Db();
$OUTPUT = new Output();
$collection = $db->selectCollection("AssetVotingAndRating");
$LF = new LuxFunctions();
$AUTH = new Auth();

$query = array(
    '_id' => $Lf->fetch_avail('object_id')
);

$results = $collection->findOne($query);

$commentThread = resolve($results['parent_ids'], "AssetVotingAndRating");
array_push($commentThread, $results);
$OUTPUT->success("comment thread resolved", $commentThread);  
