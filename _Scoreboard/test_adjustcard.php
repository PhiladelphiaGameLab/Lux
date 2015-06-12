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
//$cardID = '54d1443a1d41c83c8499e869';
//$userID = '54d24f4c1d41c8128f208388';
$cardID = $LF->fetch_avail("card_id");
$userID = $AUTH->getClientId();

/*
$results = $collection->findOne(array(
				"user_id"=>$userID
));
*/
$results = $collection->update(
    array(
	"user_id"=> $userID,
	"cards.card_id" => $cardID
	),  
    array('$set'=>array(
		"cards.$.score"=>$LF->fetch_avail("score"),
		"cards.$.last_time_modified"=> $LF->fetch_avail("last_time_modified")
	))
);

if($results['nModified']===0){
	$results = $collection->update(
	    array(
		"user_id"=> $userID
		),  
	    array('$push'=> array("cards"=>
				array(
					"score"=>$LF->fetch_avail("score"),
					"card_id"=> $LF->fetch_avail("card_id"),
					"word"=> $LF->fetch_avail("word"),
					"lesson_id"=> $LF->fetch_avail("lesson_id"),
					"last_time_modified"=> $LF->fetch_avail("last_time_modified")

				)
			)  
		));

}

$OUTPUT->success("Successfully Updated Card", $results);
?>
  
