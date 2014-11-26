<?php
include_once('../core/query.php');
// Queries for that code and sees if that is a valid code for the purpose 
// returns the resolved document for that code
query(
        array(
                "collectionName" => "CodeGenerator"
                ,"enqueue" => false
                ,"pubsub" => false
		,"aggregate" => false
		,"distinct" => false
                ,"priority" => "Low"
		,"resolve" => true
        )
);
?>

