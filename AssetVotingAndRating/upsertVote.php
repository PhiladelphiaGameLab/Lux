<?php
include_once('../core/upsert.php');
// Format a vote into a useable number for the system

/* 1.  */

upsert(
        array(
                "collectionName" => "AssetVotingAndRating"
                ,"enqueue" => false
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

