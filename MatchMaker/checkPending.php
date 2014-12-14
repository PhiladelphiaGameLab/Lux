<?php

include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db();
$OUTPUT = new Output();
$collection = $db->selectCollection("Published");
$AUTH = new Auth();
$LF = new LuxFunctions();

// query for group of range to range

$groups = $db->selectCollection("Groups");
$sizes = array();

$results = $groups->findOne(array('players' =>
                array('$elemMatch'=>array('id'=>$AUTH->getClientId()))));

if(isset($results)){
	$range = $results["range"];

	for($i=$range["low"]; $i < $range["high"]; $i++){
		$sizes[] = array("players" => array('$size' => $i));
	}
	if($range["low"] == $range["high"]){
		$sizes[] = array("players" => array('$size' => $range["low"]));
	}
	$matchingGroup = $groups->findAndModify(
			// query
			array('_id'=>$results["_id"]
				,'$or'=> $sizes)
			// update
			,array('$set' =>
				array('accepting'=> false
				,'timestamp'=>microtime())));
	if(isset($matchingGroup)){
		$OUTPUT->success("Group was found", $matchingGroup["_id"]);
	}else{
		$OUTPUT->success("No Group yet");
	}
}else{
	$OUTPUT->error("No Request is Pending");
}




