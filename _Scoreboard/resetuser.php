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

$query = array(
    "userID" => $AUTH->getClientId()
);

$collection->remove($query, array("justOne" => true));

$results = $collection->insert($query);

$OUTPUT->success("success", $results);  
