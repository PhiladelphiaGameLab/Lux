<?php
include_once('../Core/query.php');
query(
        array(
                "collectionName" => "Users"
                ,"enqueue" => false
                ,"pubsub" => false
		,"aggregate" => false
		,"distinct" => false
                ,"priority" => "Low"
		,"resolve" => true
        )
);
?>

