<?php

include_once("../Core/db.php");
include_once("../Core/lux-functions.php");
include_once("../Core/auth.php");
include_once("../Core/output.php");
$db = new Db();
$OUTPUT = new Output();
$LF = new LuxFunctions();
$AUTH = new Auth();
$query = $LF->fetch_avail("query");
$db->subscribe($query, $AUTH);
$OUTPUT->success("Successfully Subscribed to update", $query);


?>

  
