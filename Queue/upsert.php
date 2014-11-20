<?php

include_once('../core/upsert.php');
        // Add timestamp and other fields to document as needed
upsert(
	array(
		"collectionName" => "Queue"
		,"enqueue" => false
		,"pubsub" => false
		,"priority" => "High"
       )
);

?>
