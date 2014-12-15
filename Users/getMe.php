<?php
include_once('../Core/auth.php');
include_once('../Core/output.php');
$OUTPUT = new Output();
include_once('../Core/lux-functions.php');
$LF = new LuxFunctions();
if($LF->is_avail("access_token")){
	$AUTH = new Auth();
	$OUTPUT->success("Found Client Document", $AUTH->getClientInfo());
}else{
	$OUTPUT->error("Access_token not provided");
}

