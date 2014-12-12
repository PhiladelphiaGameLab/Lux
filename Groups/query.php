<?php
include_once('../Core/query.php');
query(
        array(
                "collectionName" => "Groups"
                ,"enqueue" => false
                ,"pubsub" => false
		,"aggregate" => false
		,"distinct" => false
                ,"priority" => "low"
		,"resolve" => true
        )
);
?>

