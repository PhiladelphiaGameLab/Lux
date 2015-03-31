<?php
include_once('../Core/db.php');
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/auth.php');

$db = new Db();
$OUTPUT = new Output("System");
$collection = $db->selectCollection("APIs");
$LF = new LuxFunctions();

// Get the call definition from the database
$service = $collection->findOne(array("service"=>$LF->fetch_avail("service")));

if(isset($service)){
	// base_url ? key_name = key
	// example.com?key=siofhsafd
        if(!isset($service["key_name"]) || !isset($service["key"]) || !isset($service["base_url"])){
		$OUTPUT->error(2, "Database is not properly set-up for this service");	
	}

	// the GET parameters of the call
        $call = $LF->fetch_avail("call");

        // check if the first portion of the call has a / -> if so, remove it.
        if(substr($call, 0, 1) === "/"){
                $call = substr($call, 1);
        }
	// do the same check for the base_url
        if(substr($base, -1) === "/"){
                $base = substr($base, 0,-1);
        }

	// if the call already has parameters after it, then just add the key to the end
        if(strpos($call, "?") != FALSE){
                $document = $base."/".$call."&".$service["key_name"]."=".$service["key"];
	// if the call doesn't have any parameters in the GET request, then only add the key to it 
        }else{
                $document = $base."/".$call."?".$service["key_name"]."=".$service["key"]);
        }

	// Get the post parameters
	if($LF->is_avail("params")){
		// send the post parameters
		if(is_array($LF->fetch_avail("params")){
			$postdata = http_build_query($LF->fetch_avail("params"));
		}else{
			$postdata = http_build_query(array($LF->fetch_avail("params")));
		}
	}else{
		$postdata = http_build_query(array());
	}
	// build the HTTP request
	$opts = array('http' =>
	    array(
		'method'  => 'POST',
		'header'  => 'Content-type: application/x-www-form-urlencoded',
		'content' => $postdata
	    )
	);
	// make the HTTP into a "context"
	$context  = stream_context_create($opts);

	// make the actual request in context
	$result = file_get_contents($document, false, $context);

	$OUTPUT->success(1, null, $results);

}else{
        $OUTPUT->error(2, "Service Could not be found");
}

