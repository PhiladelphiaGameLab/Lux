<?php
include_once('../core/upsert.php');
// change to add a user to the pending possible matches
// first call query and see if any matches are already possible
upsert(
        array(
                "collectionName" => "MatchMaking"
                ,"enqueue" => false
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

