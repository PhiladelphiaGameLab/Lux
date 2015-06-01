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

$results = $collection->update(
    array(
	"userId"=> $AUTH->getClientId(),
	"cards.cardID" => $LF->fetch_avail("cardID")
	),  
    array('$set'=>array(
		"score"=>$LF->fetch_avail("score"),
		"completed"=>$LF->is_avail("completed")? $LF->fetch_avail("completed") : false
	)),
    array("upsert" => true)
);
if(isset($results["writeError"])){
	$results = $collection->update(
	    array(
		"userId"=> $AUTH->getClientId(),
		),  
	    array('$push'=> array("cards"=>
				array(
					"score"=>$LF->fetch_avail("score"),
					"completed"=>$LF->is_avail("completed")? $LF->fetch_avail("completed") : false
					"cardID" => $LF->fetch_avail("cardID")
				)
			),  
		));
}

$OUTPUT->success("Successfully Updated Card", $results);
?>
  
