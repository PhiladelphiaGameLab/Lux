<?php
include_once('../core/upsert.php');
// reformat the update to remove an achivevment
upsert(
        array(
                "collectionName" => "Users"
                ,"enqueue" => true
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

  
