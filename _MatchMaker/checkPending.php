<?php

include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db();
$OUTPUT = new Output();
$AUTH = new Auth();
$LF = new LuxFunctions();

// query for group of range to range
$groups = $db->selectCollection("Groups");
$sizes = array();
$memberNumber = 0;

// find a MM where the player is listed as waiting
$results = $groups->findOne(array('players' =>
                array('$elemMatch'=>array('id'=>$AUTH->getClientId()))));

// Make sure they are waiting
// This should only be false if they
// Have not previously made a request
if(isset($results)){	
	
	// create the players size object
	// ensures that the match is ready 
	// loops from range low to high
	$range = $results["range"];
	// if low == high, it will only create 1 entry in the array
	for($i=$range["low"]; $i <= $range["high"]; $i++){
		$sizes[] = array("players" => array('$size' => $i));
	}

	// query for the document again, but make sure the size 
	// is in the range
	$matchingGroup = $groups->findAndModify(
			// query
			array('_id'=>$results["_id"]
				,'$or'=> $sizes)
			// update
			,array('$set' =>
				// make sure it is not accepting any more participants
				array(
					'accepting'=> false
					// reset the timestamp
					,'timestamp'=>microtime()
					// incremement the number of participants by 1
				)
			,'$addToSet' => array("participants" => array('id' =>$AUTH->getClientId()))
			)
			// fields
			,null	
			// options
			//,array("update"=>"TRUE", "new"=>"true")
			,array("new"=>"true")
	);

	// if the previous query returned true, then the proper number of people are in the results
	if(isset($matchingGroup)){

		// Find where in the Array they are
		// Just loop until you find their ClientId
		foreach($matchingGroup["players"] as $playerId){
			if($playerId["id"] == $AUTH->getClientId()){
				break;
			}
			$memberNumber += 1;
		}

		$OUTPUT->success(array("group" => $matchingGroup["_id"], "member"=>$memberNumber));
		// if all the players are participants
		if(sizeof($matchingGroup["participants"]) == sizeof($matchingGroup["players"]) && !$matchingGroup["accepting"]){
			// remove the group now
			$groups->remove(array('_id'=>$results["_id"]));
		}
	}else{
		// 
		$OUTPUT->success(array("pending" => true));
	}
}else{
	$OUTPUT->error("No Request is Pending");
}




  
