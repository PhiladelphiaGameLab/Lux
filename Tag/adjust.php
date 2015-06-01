<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db("Assets");
$OUTPUT = new Output();
$LF = new LuxFunctions();
$collectionName = $LF->is_avail("coll") ? $LF->fetch_avail("coll") : "Standard";
$collection = $db->selectCollection($collectionName);
$AUTH = new Auth();

// { $addToSet: { tags: { $each: [ "camera", "electronics", "accessories" ] } } }
$tags = $LF->fetch_avail("tags");
if(is_array($tags)){
	$update = array('$addToSet' => array('tags' => array('$each' => $tags)));
}else{
	$update = array('$addToSet' => array('tags' => $tags));
}

$options["upsert"] = true;
$options["multiple"] = false;
$options["new"] = true;
$query = array("_id" => $LF->fetch_avail("id")); 
$results = $collection->update($query, $update, $options);
$document = $collection->findOne($query);
unset($document["lux_info"]);
$OUTPUT->success(0,$document, $results);

?>

  
