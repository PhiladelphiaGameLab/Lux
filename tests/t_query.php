<?php
include_once("../core/query.php");

$testparams = array("collectionName" => "test_Assets", "pubsub" => true, "query" => array("gameid" => "3"));

query($testparams);
?>
