<?php

include_once('../core/upsert.php');
        // Add timestamp and other fields to document as needed
// TODO: Format the Query to just unsubscribe a user from a document
upsert(
	array(
		"collectionName" => "PubSub"
		,"enqueue" => false
		,"pubsub" => false
		,"priority" => "Low"
       )
);

?>
