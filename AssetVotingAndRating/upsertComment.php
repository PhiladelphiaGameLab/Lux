<?php
include_once('../core/upsert.php');
// Set so that it is foramtted as a comment and then upsert it into the database
upsert(
        array(
                "collectionName" => "AssetVotingAndRating"
                ,"enqueue" => true
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

