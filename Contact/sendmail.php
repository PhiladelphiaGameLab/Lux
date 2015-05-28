<?php
include_once('../Core/lux-functions.php');
include_once('../Core/output.php');
include_once('../Core/db.php');
include_once('../Core/auth.php');

$db = new Db("System");
$OUTPUT = new Output();
$collection = $db->selectCollection("Contact");
$LF = new LuxFunctions();

$query = array("email_id" => $LF->fetch_avail("email_id")); 
$document = $collection->findOne($query);


$to = trim(implode(" , ", $document["addresses"]), ' , ');
$subject = $LF->fetch_avail("subject");
$message = $LF->fetch_avail("body");


$sender = $LF->is_avail("sender") ? $LF->fetch_avail("sender") : "noreply@example.com";

$headers = 'From: '.$sender . "\r\n" .
           'Reply-To: '. $sender . "\r\n" .
           'X-Mailer: PHP/' . phpversion();

$result = mail($to, $subject, $message, $headers);

if($result == 1){
	$OUTPUT->success(0,null, null);
}else{
	$OUTPUT->error(2,"An Error occured in the mail function");
}

?>

