<?php
require('../core/Auth.php');
require('../core/db.php');
require('../core/output.php');

/*
Params in: None
        
Ouput: 
        
Functionality: Queries collection with passed query string
        
*/
function query(){
	//Standard setup and checks
	$AssetDatabase = $DB->selectCollection("AssetDatabase");
	$Queue = $DB->selectCollection("Queue");
	$PubSub = $DB->selectCollection("PubSub");
	$clientDoc = $AUTH->getClientInfo(); // this is a php array
	$client_id = $AUTH->getClientId(); // This is a mongo id 
	$client_groups = $AUTH-getClientGroups(); // This is a php array of Mongo Ids

	// check for variable existance
	$id = is_avail("id"); // in output.php
	// $id = is_post("id");
	// $id = is_get("id");
	$query = array();
	foreach($_GET as $key => $value){
	    $query[$key] = $value;
	}

	$cursor = $AssetDatabase->find($query);

	$result = iterator_to_array($cursor);

	echo json_encode($result);
}
// write to Asset Database with Appropriate Query

// if Query(), call Subscribe Function

// otherwise (and not copy), Add update to Mongo Queue

// if copy, then copy the entire group over to a new group you created.

// print output 


//$OUTPUT->success("");
//$OUTPUT->printArray();
//$OUTPUT->addToPrintArray();
?>
