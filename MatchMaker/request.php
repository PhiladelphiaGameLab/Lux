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

$range = $LF->fetch_avail("range");
$groups = $db->selectCollection("Groups");
$sizes = array();
for($i=0; $i < $range["high"]; $i++){
	$sizes[] = array("players" => array('$size' => $i));
}
$matchingGroup = $groups->update(
		// query
		array("range.high"=> array('$lt' => $range["high"]+1)
			,"range.low" => array('$gt' => $range["low"]-1)
			,"accepting"=>true
			,'$or'=> $sizes)
		// update
		,array('$set' => 
			array('accepting'=> true, 'range' => array('low'=>$range["low"], 'high'=>$range['high'])
			,'timestamp'=>microtime())
			,'$addToSet' => array("players" => array('id' =>$AUTH->getClientId())))
		// options
		,array("upsert"=>true)
		);

if(isset($matchingGroup)){
	$OUTPUT->success("We have added you to the waiting list for a group");
}else{
	$OUTPUT->error("Something has gone wrong");
}
