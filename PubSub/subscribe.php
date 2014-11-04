<?php

include_once('../core/upsert.php');
// Add timestamp and other fields to document as needed
// TODO: Format the query to just subscribe the user to a document
upsert(
	array(
		"collectionName" => "PubSub"
		,"enqueue" => false
		,"pubsub" => false
		,"priority" => "Low"
       )
);

?>
