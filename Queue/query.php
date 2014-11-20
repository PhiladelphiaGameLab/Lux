<?php

include_once('../core/query.php');
query(
	array(
		"collectionName" => "Queue"
		,"enqueue" => false
		,"pubsub" => true
		,"aggregate" => false
		,"distinct" => false
		,"priority" => "Low"
		,"resolve" => false
	)
);


?>
