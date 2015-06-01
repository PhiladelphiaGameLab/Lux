<?php
include_once('../core/upsert.php');
// format a query that associates an existing code with a group/user
upsert(
        array(
                "collectionName" => "CodeGenerator"
                ,"enqueue" => false
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

  
