<?php
include_once('../core/upsert.php');
upsert(
        array(
                "collectionName" => "Groups"
                ,"enqueue" => true
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

