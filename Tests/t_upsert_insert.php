<?php
include_once("../core/upsert.php");

$testparams = array("collectionName" => "test_Assets", "insert"=>"true", "doc" => array("color" => "red", "group"=>"544aaa8be43ca55e86db0d54"));

//$testparams = array("collectionName" => "test_groups", "doc" => array("name" => "copy_test" , "random"=>"data"));
//$testparams = array("collectionName" => "test_Assets", "doc" => array("gameid" => "3","group_id" => "555"));

upsert($testparams);

?>
