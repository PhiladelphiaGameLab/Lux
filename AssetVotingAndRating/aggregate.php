<?php
include_once('../core/query.php');
// format a query for that particular asset
query(
        array(
                "collectionName" => "AssetsVotingAndRating"
                ,"enqueue" => false
                ,"pubsub" => false
		,"aggregate" => true
		,"distinct" => false
                ,"priority" => "Low"
		,"resolve" => true
        )
);
?>

