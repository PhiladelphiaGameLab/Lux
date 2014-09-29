<?php
include_once('../core/query.php');
query(
        array(
                "collectionName" => "Groups"
                ,"enqueue" => false
                ,"pubsub" => true
		,"aggregate" => false
		,"distinct" => false
                ,"priority" => "High"
		,"resolve" => true
        )
);
?>

