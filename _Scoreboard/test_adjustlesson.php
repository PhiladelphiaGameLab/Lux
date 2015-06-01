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
	"user_id"=> $AUTH->getClientId(),
	"lessons.lesson_id" => $LF->fetch_avail("lesson_id")
	),  
    array('$set'=>array(
		"lessons.$.score"=>$LF->fetch_avail("score"),
		"lessons.$.last_time_modified"=> $LF->fetch_avail("last_time_modified"),
		"lessons.$.completed"=>$LF->is_avail("completed")? $LF->fetch_avail("completed") : false
	))
);
if($results['nModified']===0){
	$results = $collection->update(
	    array(
		"user_id"=> $AUTH->getClientId(),
		),  
	    array('$push'=> array("lessons"=>
				array(
					"score"=>$LF->fetch_avail("score"),
					"completed"=>$LF->is_avail("completed")? $LF->fetch_avail("completed") : false,
					"lesson_id" => $LF->fetch_avail("lesson_id"),
					//"lesson_name"=>$LF->fetch_avail("lesson_name"),
					"last_time_modified"=>$LF->fetch_avail("last_time_modified")
				)
			),  
		));
}

$OUTPUT->success("Successfully Updated Level", $results);
  
