<?php
include_once('../Core/auth.php');
include_once('../Core/output.php');
$OUTPUT = new Output();
include_once('../Core/lux-functions.php');
$LF = new LuxFunctions();
$AUTH = new Auth();
$document = $AUTH->getClientInfo();
unset($document["lux_info"]);
$OUTPUT->success(1, $document, array());

  
