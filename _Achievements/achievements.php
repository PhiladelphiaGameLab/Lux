<?php



echo "start" . "<br/>";
echo "<br/>";


function grantAch($user_id,$ach_id){
	//setting mongo obj
	$m = new MongoClient();
	$db = $m->ach;
	$collAch = $db->ach;
	$collUserAch = $db->user_ach;

	//getting required user id form the logic framke work call pramas.
	$user_id = 'userach1';

	//this variable contains the ach id of the ach that is being assigned to the user
	$ach_id ='ach1';

	// query for specific usr
	$userQuery = array('$and' => array(array('_id' => $user_id),array('ach.ach_id'=>$ach_id)));
	$userQueryField = array('ach'=>array('$elemMatch' => array('ach_id'=>$ach_id)));
	
	//getting user ach result
	$ures = $collUserAch->find($userQuery,$userQueryField);

	//printing for testing code
	printData($ures);

	/*
	
	// Once this results set is done the next tasks are
	// 1. Checking if the ach is Repetable (with ach_id its just an if stmt)
	// 2. Checking the Quatity Acquired with that ach has reached the thresold (Max) value
	//	2.1 if value reached assign the ach
	//	2.2 if lesser increment the qty acq and update in DB
	

	// For the first time when the ach is targeted it is incremented and assigned to the user

	*/





	$m->close();
}

function printData($ures){
	// print all data
	foreach ($ures as $key ) {
		echo "<br/>";
		var_dump($key);

		echo "<br/>" . $key['_id'];
		echo "<br/>" . $key['ach'][0]['ach_id'];
		
	}
}


grantAch($user_id,$ach_id);



echo "<br/>" . "done";

?>  
