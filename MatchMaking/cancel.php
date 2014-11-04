<?php
include_once('../core/upsert.php');
// change query to remove user from pending list
upsert(
        array(
                "collectionName" => "MatchMaking"
                ,"enqueue" => false
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

