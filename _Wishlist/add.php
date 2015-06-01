<?php

include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db();
$OUTPUT = new Output();
$collection = $db->selectCollection("Wishlist");
$LF = new LuxFunctions();
$AUTH = new Auth();

$wishlistName = $LF->is_avail("wishlistName")? $LF->fetch_avail("wishlistName") : "wishlist";
$results = $collection->update(
    array(
	"userId"=> $AUTH->getClientId(),
	),  
    array('$addToSet'=>array(
		$wishlistName => $LF->fetch_avail("movieId");
	)),
    array("upsert" => true)
);

$OUTPUT->success("Successfully Updated Card", $results);
?>

  
