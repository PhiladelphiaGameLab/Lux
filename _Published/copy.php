<?php

include_once('../core/copy.php');
copyDocument(
	array(
		"collectionName" => "Queue"
		,"R" => false
		,"updates" => false
		,"pubsub" => false
		,"enqueue" => false
		,"priority" => "Low"
	)
);

?>
  
