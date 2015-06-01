<?php
include_once('../core/query.php');
query(
        array(
                "collectionName" => "Assets"
                ,"enqueue" => false
                ,"pubsub" => true
		,"aggregate" => false
		,"distinct" => false
                ,"priority" => "High"
		,"resolve" => false
        )
);
?>

  
