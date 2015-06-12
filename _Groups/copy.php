<?php
include_once('../core/copy.php');
copyDocument(
        array(
                "collectionName" => "Groups"
		,"R" => array(
				"newGroup" => true
				,"updates" => false
				,"collectionName" => "Assets"
				,"enqueue" => false
				,"priority" => "Low"
				,"pubsub" => false
			)
		)
);
//                 ,"updates" => false
//                 ,"pubsub" => false
// 		,"enqueue" => true
//                 ,"priority" => "High"
//         )
?>

  
