<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db("System");
$OUTPUT = new Output();
$collection = $db->selectCollection("Users");
$AUTH = new Auth();
$LF = new LuxFunctions();


// This section still needs to be completed
// This function needs to worry about validating only 1 parameter at a time,
// and need to be able to return the "Deep" results too- instead of returning
// a bool for deep results, return the shortcode being used (for email or sms- for CC/url true or false will suffice).
function validate($value, $validation, $param, $deep){
	switch($validation){
		case "len.min":
			return true;
		default:
			return true;
	}
}


// This section is completed, the values are successfully acquired and passed to the above function
// and the output from the function is properly handled. 
$values = $LF->fetch_avaiL("values");
if(!is_array($values)){
	$OUTPUT->error(2, "values passed in needs to be an array");
}
$results_all = array();
foreach($LF->fetch_avail("values") as $values){
	$results["key"] = isset($values["key"])? $values["key"] : $OUTPUT->error(0, "Key is required");
	$results["validation"] = isset($values["validate"])? $values["validate"] : $OUTPUT->error(0, "A validation method (validate) is required");
	$results["required"] = isset($values["required"]) ? $values["required"] : false;
	$results["deep"] = isset($values["deep"]) ? $values["deep"] : false;
	$results["params"] = isset($results["params"])? $values["params"] : null;
	if($results["required"]){
		$results["value"] = isset($values["value"])? $values["value"] : $OUTPUT->error(0, "A Value is required for this key");
		$results["valid"] = validate($results["value"], $results["validation"], $results["params"], $results["deep"]);
	}else{
		$results["value"] = isset($results["value"])? $values["value"] : null;
		if($results["value"] == null){
			$results["valid"] = true;
		}else{
			$results["valid"] = validate($results["value"], $results["validation"], $results["params"], $results["deep"]);
		}
	}


	$results_all[] = $results;
		   // add the validation results,
		   // for deep validations (for instance a cell number, return the shortcode that was texted to the phone)
					 //(This allows the short code to be verified on the front-end, allowing the developer
}
$OUTPUT->success(0,null, $results_all);

?>

  
