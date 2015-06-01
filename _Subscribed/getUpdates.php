<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db();
$OUTPUT = new Output();
$collection = $db->selectCollection("Assets");
$AUTH = new Auth();
$LF = new LuxFunctions();


$subscribers = $db->selectCollection("Subscribers");
$published = $db->selectCollection("Published");
$documents = array();

$subscriptions = $subscribers->find(array("subscribers"=> array(
			'$elemMatch'=>array('id'=>$AUTH->getClientId()))));


// get Subscriptions
foreach($subscriptions as $subscription){
	if(isset($subscription["query"]) && is_array($subscription["query"])){
		$query = $subscription["query"];
		echo $query;
		$query["timestamp"] = array('$gt' => $LF->fetch_avail("since"));
		$documents[] = $published->find($query);
	}
}
$OUTPUT->success("Found Published Material", $documents);
?>

  
