<?php
include_once("../core/copy.php");

$testparams = array("collectionName" => "test_groups", "id" => "544aaa8be43ca55e86db0d54", "R" => array("collectionName" => "test_Assets","newGroup" =>"true", "query" => array("color" => "red"), "updates" => array("color" => "yellow")));

copyDocument($testparams);
?>
