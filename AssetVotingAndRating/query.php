<?php

// This is called somewhere else and can't delete

include_once('../core/query.php');
query(
        array(
                "collectionName" => "AssetVotingAndRating"
                ,"enqueue" => false
                ,"pubsub" => true
		,"aggregate" => false
		,"distinct" => false
                ,"priority" => "Low"
		,"resolve" => false
        )
);
?>

