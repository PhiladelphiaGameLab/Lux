<?php
include_once('../core/query.php');
// format a query to pull out if the user has been put into a match
query(
        array(
                "collectionName" => "MatchMaking"
                ,"enqueue" => false
                ,"pubsub" => false
		,"aggregate" => false
		,"distinct" => false
                ,"priority" => "Low"
		,"resolve" => false
        )
);

// get the results of the query and see if the user has been put into a match
// upsert to remove the match once all players are in the match

?>

