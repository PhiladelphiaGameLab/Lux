<?php
include_once('../Core/upsert.php');
upsert(
        array(
                "collectionName" => "Users"
		,"SN" => true 
                ,"enqueue" => false
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

