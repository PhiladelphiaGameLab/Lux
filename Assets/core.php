<?php
// Includes the core modules of lux
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../UserAssetOwnership/userAssetOwnership.php');
include_once('../Core/auth.php');

// creates standard classes for the core modules
$OUTPUT = new Output();
$LF = new LuxFunctions();

// Select the proper db and collection
$db = new Db("Assets");
$collectionName = $LF->is_avail("coll") ? $LF->fetch_avail("coll") : "Standard";
$collection = $db->selectCollection($collectionName);

// This should be included in all Asset Management API files
  
