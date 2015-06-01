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
	"levels.levelID" => $LF->fetch_avail("levelID")
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
	    array('$push'=> array("levels"=>
				array(
					"score"=>$LF->fetch_avail("score"),
					"completed"=>$LF->is_avail("completed")? $LF->fetch_avail("completed") : false
					"levelID" => $LF->fetch_avail("levelID")
				)
			),  
		));
}

$OUTPUT->success("Successfully Updated Level", $results);
  
