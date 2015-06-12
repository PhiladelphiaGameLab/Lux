<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');
$db = new Db();
$OUTPUT = new Output();
$collection = $db->selectCollection("Scoreboard");
$LF = new LuxFunctions();
$AUTH = new Auth();
//$userID = $AUTH->getClientId();
$userID = '54d24f4c1d41c8128f2083e2';
//$cardID = $LF->fetch_avail('card_id');
$cardID = '';
$cardScore = $LF->fetch_avail('score');
$query = array(
    'user_id' => $userID
   // 'cards.card_id' => $cardID
);
$update = array(
    '$set' => array(
        'cards.$.score' => $cardScore
    )
);
$options = array(
    'upsert' => true
);
$results = $collection->findOne($query);
//$results = $collection->update($query, $update, $options);
$OUTPUT->success("success", $results);
?>
  
