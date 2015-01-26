<?php

include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db();
$OUTPUT = new Output();
$collection = $db->selectCollection("AssetVotingandRanking");
$LF = new LuxFunctions();
$AUTH = new Auth();
$userID = $AUTH->getClientId();
$id = new MongoId();

if ($LF->fetch_avail('comment_type') == 'new') {
    $base_comment_id = $id;
    $comment_depth = 0;
    $parent_id_array = array(
        $id
    );
} else {
    $base_comment_id = $LF->fetch_avail('base_comment_id');
    $comment_depth = intval($LF->fetch_avail('comment_depth')) + 1;
    $parent_id_array = $LF->fetch_avail('parent_id_array');
    array_push($parent_id_array, $id);
}

$newcomment = array(
    '_id' => $id,
    'user_id' => $AUTH->getClientId(),
    'comment_body' => $LF->fetch_avail('comment_body'),
    'comment_depth' => $comment_depth,
    'parent_id' => $LF->fetch_avail('parent_id'),
    'parent_id_array' => $parent_id_array,
    'base_comment_id' => $base_comment_id,
    'date' => new DateTime('now'),
    'score' => 0,
    'upvotes' => 0,
    'downvotes' => 0,
    'replies' => array()
);

if ($LF->fetch_avail('comment_type') == 'new') {
    $results = $collection->insert($newcomment);
} else {
    $results = $collection->findOne(
        array(
            '_id' => $base_comment_id
        )
    );
    $results = $results['replies'];
    for ($i = 0; $i <= $comment_depth; $i++) {
        foreach ($results as $comment) {
            if (in_array($comment['_id'])){
                echo 'test'; // not done
            }
        }
    }
}

$OUTPUT->success("comment successfully inserted", $results);