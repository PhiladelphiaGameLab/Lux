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

if ($LF->is_avail('object_type')) {
    if ($LF->fetch_avail('object_type') == 'asset') {
        $results['comment_ids'] = resolve($results['comment_ids'], "AssetVotingAndRating");
        $OUTPUT->success("object resolved", $results);
    } elseif ($LF->fetch_avail('object_type') == 'comment') {
        $results['parent_id'] = resolve($results['parent_id'], "AssetVotingAndRating");
        $results['asset_id'] = resolve($results['asset_id'], "AssetVotingAndRating");
        $results['parent_array'] = resolve($results['parent_array'], "AssetVotingAndRating");
        $OUTPUT->success("object resolved", $results);
    } else {
        $OUTPUT->error("object type not valid", $results);
    }
} else {
    $OUTPUT->error("object type not input", $results);
}