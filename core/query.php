<?php
require('../core/Auth.php');
require('../core/db.php');
require('../core/output.php');

/*
Params in: 
	$params = array(
		collectionName: the name of the collection you would like to change
		_id: id of the document you would like to aquire
		or
		query: the Query you would like to run
	)
        
Ouput: 
        Outputs an array of documents
Functionality: 
	Queries collection with passed query string
        
TODO:
	uses is_avail instead of isset
	Push into the pub/sub section
	Format on a single parameters matching a description
	Accept $gt, $lt, $rad, $box, and $btw operators
	Specific Limit and Ordering Filters
	Queries can be Filtered by something	
	//Allow for Aggregation as well
	//Queries for Distinct
	//Automatically Resolve DBRefs
	Use Grouping where availiable
*/
function query($params){

	if(isset($params["distinct"]) && $params["distinct"]){
		// use distinct instead of find
	}else if(isset($params["aggregate"]) && $params["aggregate"])){
		// use aggregate instead of  find
	}
	
	if(isset($params["resolve"]) && $params["resolve"]){
		// loop through the documents and resolve the dbRefs
	}

	
}


function query($params){
	// open the neccisary functions
	$db = new Db();	
	$OUTPUT = new Output();
	$coll = $db->selectCollection($params["collectionName"]);
	$AUTH = new Auth();	
	// switch based on if _id or Query is set
	if(isset($params["_id"])){
		// Query based on _id
		// should pobably use findOne here
		$cursor = $coll->find($params["_id"]);
	}else if(isset($params["query"])){
		// Query based on a Query that is passed in
		$cursor = $coll->find($params["query"]);
	}
	
	// Convert iterator to an array for output
	$result = iterator_to_array($cursor);
	// $output the iterator array
	$OUTPUT->success("The Mongo query was successful",$result);
	
	// TODO:
	// add to pub/sub for this user
}
?>
