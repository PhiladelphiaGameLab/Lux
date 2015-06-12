<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

function getArbitrary($results, $fields){
	if(count($fields) == 1){
		return $results[array_shift($fields)];
	}else{
		$rest = $results[reset($fields)];
		$field_new = array_shift($fields);
		return getArbitrary($rest, $fields);
	}
}

function getField($field){
	$fields = explode(".",$field);
	$query = array("field" => $fields[0]); 
	$db = new Db("System");
	$collection = $db->selectCollection("CMS");
	$results = $collection->findOne($query);
	array_shift($fields);
	return getArbitrary($results, $fields);
}
	

function getPage($template){
	$re = "/{{(.*)}}/miU"; 
	$content = file_get_contents( "./html/".$template, true );
	preg_match_all($re, $content, $matches, PREG_PATTERN_ORDER);
	$content_values = array();
	foreach($matches[1] as $value){
		$content_values[] = getField($value);
	}
	return str_replace($matches[0], $content_values, $content);

}


$LF = new LuxFunctions();

if(is_array($LF->fetch_avail("template"))){
	foreach($LF->fetch_avail("template") as $value){
		$replaced = getPage($value);
		echo $replaced;
	}
}else{
	$replaced = getPage($LF->fetch_avail("template"));
	echo $replaced;
}


?>

  
