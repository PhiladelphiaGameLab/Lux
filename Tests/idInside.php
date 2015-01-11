<?php
include('../Core/db.php');
$DB = new Db();

$collection = $DB->selectCollection("Published");

$collection->insert(array("test" => "test2", "test3" => array("test4" => "test5", "commentId" => new MongoId())));

