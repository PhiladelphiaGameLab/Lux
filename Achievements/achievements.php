<?php 

try{

//Mongo Db conn and the required collections
$conn = new MongoClient();
$db = $conn->achievments;
$collAch = $db->achievments; 
$collUserAch = $db->user_achivements;


/*
* Main funtion to be called by LF
* Can be given Pramas as user_id,DB,Query,Upsert..

*/

function giveAchievements(){
	
	//check if user has that ach, getting user id from pramas and replacing it with userach2
	
	$userQry= array('_id' => 'userach2');
	$chkUserAch = $collUserAch->find({$userQry);
	
	//Displaying result set for one user. not to be included just testing
	foreach ($c as  $val) {
	    var_dump($val);
	}
	/*Matching Required Qty with Acheieved Qty
		if RQ eqls AQ achievment is awarded
	
		//Check if award can be given multiple times
	
		//TO DO code	
		

		else RQ ++
	*/
	

	
}


$conn->close();
} 
catch (MongoConnectionException $e) {
  die('Error connecting to MongoDB server');
} 
catch (MongoException $e) {
  die('Error: ' . $e->getMessage());
}
?>
