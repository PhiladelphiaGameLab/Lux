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
$memberNumber = 0;

if(isset($results)){	
	
	foreach($results["players"] as $playerId){
		if($playerId["id"] == $AUTH->getClientId()){
			break;
		}
		$memberNumber += 1;
	}

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
		$OUTPUT->success(array("group" => $matchingGroup["_id"], "member"=>$memberNumber));
	}else{
		$OUTPUT->success(array("pending" => true));
	}
}else{
	$OUTPUT->error("No Request is Pending");
}




