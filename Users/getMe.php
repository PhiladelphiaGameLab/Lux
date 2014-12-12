<?php
include_once('../Core/query.php');
if(isset($_POST["access_token"]){
	$AUTH = new Auth();
	echo "<pre>"
		.$AUTH->getClientInfo().
	     "</pre>";
}

