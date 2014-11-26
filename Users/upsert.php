<?php
include_once('../core/upsert.php');
upsert(
        array(
                "collectionName" => "Users"
                ,"enqueue" => false
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

