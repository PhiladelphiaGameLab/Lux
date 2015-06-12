<?php

include_once("../Core/db.php");
include_once("../Core/lux-functions.php");
include_once("../Core/auth.php");
include_once("../Core/output.php");
$db = new Db();
$OUTPUT = new Output();
$LF = new LuxFunctions();
$AUTH = new Auth();
$update = $LF->fetch_avail("update");
$OUTPUT->success("Successfully Published Document", $db->publish($update, $AUTH));


?>
  
