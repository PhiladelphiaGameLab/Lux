<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db("System");
$OUTPUT = new Output();
$collection = $db->selectCollection("CMS");
$LF = new LuxFunctions();

$fields = $LF->fetch_avail("field");
if(is_array($fields)){
	$query = array("field" => array('$in' => $fields)); 
	$document = $collection->find($query);
	foreach($document as $key=> $value){
		unset($value["lux_info"]);
	}
}else{
	$query = array("field" => $fields); 
	$document = $collection->findOne($query);
	unset($document["lux_info"]);
}
$OUTPUT->success(0,$document, null);

?>

  
