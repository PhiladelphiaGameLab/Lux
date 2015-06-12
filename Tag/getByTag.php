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

// Find the items that have any of these elements in their tags set
$tags = $LF->fetch_avail("tags");
if(!is_array($tags)){
	$tags = array($tags);
}
$query = array("tags"=> array('$elemMatch' => array( '$in' => $tags)));

$document = $collection->find($query);
//unset($document["lux_info"]);
$OUTPUT->success(0,$document, null);

?>

  
