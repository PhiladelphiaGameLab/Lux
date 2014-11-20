<?php
include_once("../core/upsert.php");
$testparams = array("collectionName" => "test_Assets", "doc" => array("this is new" => "so is this", "group_id" => "555"), "id" => "542ed9eee43ca55e86db0d50");

upsert($testparams);

?>
