<?php

include_once('../core/query.php');
query(
	array(
		"collectionName" => "PubSub"
		,"enqueue" => false
		,"pubsub" => false
		,"aggregate" => false
		,"distinct" => false
		,"priority" => "Low"
		,"resolve" => false
	)
);


?>
