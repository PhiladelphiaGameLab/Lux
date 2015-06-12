<?php
include_once('../core/upsert.php');
// Create a code and attach it to the given Group/User or just to a functionality
// Code can be used to verify that it is a certain user, we should be able to save the IP of the device being used to generate the code
upsert(
        array(
                "collectionName" => "CodeGenerator"
                ,"enqueue" => false
                ,"pubsub" => false
                ,"priority" => "Low"
        )
);
?>

  
