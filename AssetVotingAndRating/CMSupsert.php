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
    'content_area' => $Lf->fetch_avail('content_area')
);

$results = $collection->update(
    $query,
    $something,
    $options = array(
        'upsert' => true
    )
);

$OUTPUT->success('query successful', $results);