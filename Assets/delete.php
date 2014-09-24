<?php
// This seems done?

require('../core/auth.php');
require('../core/db.php');
require('../core/output.php');
require('../core/lux-functions.php');

/*
Params in: $target_id - the id of the document caller aims to delete
        
Ouput: success or failure
        
Functionality: deletes desired document with the specified id
        
*/

//Standard setup and checks
$AssetCollection = $DB->selectCollection("Assets");
$client_id = $AUTH->getClientId(); // This is a mongo id 
$client_groups = $AUTH-getClientGroups(); // This is a php array of Mongo Ids
$priority = is_avail("priority", false)? is_avail("priority") : "Low";


$qres = $AssetCollection->findOne(array('_id' => new MongoID($target_id)));
$rembool = false;
foreach($client_groups as $value){
	if($qres["group_id"] == $value){
		$rembool = true;
	}
}
if($rembool){
	$AssetCollection->remove(array('_id' => new MongoID($target_id)));
	$OUTPUT->success("Successfully Removed Document", array("_id" => $target_id));
	$DB->enQueue($target_id, $client_id, false, $priority); // in lux-functions
}
else{
	$OUTPUT->error("Failed to Remove Document", array("message" => "Permissions Denied", "_id" => $target_id));
}

?>
