<?php
include_once('../core/upsert.php');
upsert(
        array(
                "collectionName" => "AssetVotingAndRating"
                ,"enqueue" => false
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

