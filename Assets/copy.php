<?php
include_once('../core/copy.php');
copyDocument(
        array(
                "collectionName" => "Assets"
                ,"updates" => false
                ,"pubsub" => false
		,"enqueue" => true
                ,"priority" => "High"
        )
);
?>

