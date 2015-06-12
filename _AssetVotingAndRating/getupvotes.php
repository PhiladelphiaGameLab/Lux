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

$doc = $collection->findOne($query);

$results = count($doc['upvotes']);

$OUTPUT->success("upvotes quantified", $results);  
