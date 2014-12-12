<?php
include_once('../Core/query.php');
query(
        array(
                "collectionName" => "Assets"
                ,"enqueue" => false
                ,"pubsub" => true
		,"aggregate" => false
		,"distinct" => false
                ,"priority" => "High"
		,"resolve" => false
        )
);
?>

