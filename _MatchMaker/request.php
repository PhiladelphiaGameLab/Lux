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
$range = $LF->fetch_avail("range");
$groups = $db->selectCollection("Groups");
$sizes = array();

// Set the size variables to be anything less than "high"
for($i=0; $i < $range["high"]; $i++){
	$sizes[] = array("players" => array('$size' => $i));
}
$matchingGroup = $groups->update(
		// query
			// make sure the MMs high is <= to 
			// this requests high
		array("range.high"=> array('$lt' => $range["high"]+1)
			// Make sure the MMs Low is >= to
			// this requests low
			,"range.low" => array('$gt' => $range["low"]-1)
			// Make sure the MM is still accepting
			,"accepting"=>true
			// Make sure the MMs number of players
			// is less than this requests High
			,'$or'=> $sizes
		)
		// update
		,array('$set' => 
			array('accepting'=> true
			,'range' => array('low'=>$range["low"], 'high'=>$range['high'])
			,'timestamp'=>microtime()
			,'participants' => array()
			)
		,'$addToSet' => array("players" => array('id' =>$AUTH->getClientId()))
		)
		// options
		,array("upsert"=>true)
		);

if(isset($matchingGroup)){
	$OUTPUT->success("We have added you to the waiting list for a group");
}else{
	$OUTPUT->error("Something has gone wrong");
}
  
