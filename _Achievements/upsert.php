<?php
include_once('../core/upsert.php');
// make the achievement into something that can be processed by the system
upsert(
        array(
                "collectionName" => "Achievements"
                ,"enqueue" => true
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

  
