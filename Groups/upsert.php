<?php
include_once('../Core/upsert.php');
upsert(
        array(
                "collectionName" => "Groups"
		,"SN" => true
                ,"enqueue" => false
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

