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

$groups = $db->selectCollection("Groups");
$results = $groups->update(array('players' =>	
		array('$elemMatch'=>array('id'=>$AUTH->getClientId()))), 
		array('$pull'=>array('id'=>$AUTH->getClientId(), 'dummy'=>'Test')));
if(isset($results)){
	$OUTPUT->success("Sucessfully Unsubscribed");
}else{
	$OUTPUT->error("Something Went wrong");
}


  
