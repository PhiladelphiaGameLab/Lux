<?php
include_once('../core/upsert.php');
// reformat query to the right thing to add an achievement to the user 
upsert(
        array(
                "collectionName" => "Users"
                ,"enqueue" => true
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

  
