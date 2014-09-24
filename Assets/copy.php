<?php

require('../core/Auth.php');
require('../core/db.php');
require('../core/output.php');

/*
Params in:
	group_id to copy
	new group_name (unique)  
	old group_name
	
Ouput: 
	new group document        
Functionality:
        creates a new group document
	copies the old group into the new group section
*/

$AssetCollection = $DB->selectCollection("Assets");
$GroupCollection = $DB->selectCollection("Groups");

// check for variable existance
$group_id = is_avail("group_id"); // in output.php
$newGroupName = is_avail("new_name", false);
$newGroupPermissions = is_avail($permissions, false);

// Look up old group and see what the permission are 
$oldGroupDoc = $GroupCollection->findOne(array("_id" => new MongoId($group_id)));
if(isset($oldGroupDoc)){
	if(!$oldGroupDoc["permissions"]["copyable"]){
		$OUTPUT->error("You do not have permissions to copy this Group", array("_id" => $group_id));	
	}
}

if($newGroupName){
	$sameName = $GroupCollection->findOne(array("group_name" => $group_name));
	if(isset($sameName)){
		$OUTPUT->error("The group name already exists", array("_id" => $sameName["_id"]));
	}
	$oldGroupDoc["group_name"] = $newGroupName;
}else{
	unset($oldGroupDoc["group_name"];
}

// create new group
if($permissions){
	$oldGroupDoc["permissions"] = $permissions;
}
$oldGroupDoc["_id"] = new MongoId();
$newGroup_id = $oldGroupDoc["_id"];
$GroupCollection->insert($oldGroupDoc);

// copy all documents with new group ID
$cursor = $AssetCollection->find(array("group_id" => new MongoId($group_id)));
foreach($cursor as $doc){
	$doc["group_id"] = $newGroup_id;
	$AssetCollection->insert($doc);
}

enQueue($newGroup_id, $client_id, $newGroupDoc, "High");
$OUTPUT->success("New Group Created", array("group document" => $oldGroupDoc));
?>
