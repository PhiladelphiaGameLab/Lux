<?php
include_once('../core/upsert.php');
upsert(
        array(
                "collectionName" => "Assets"
                ,"enqueue" => true
                ,"pubsub" => false
                ,"priority" => "High"
        )
);
?>

