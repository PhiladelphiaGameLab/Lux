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

$results = $collection->find(
    array(
	"userId"=> $AUTH->getClientId(),
	"cards.cardID" => $LF->fetch_avail("cardID")
	),
	array(
		'cards.$'=>1	
	)  
);

$OUTPUT->success($results);
