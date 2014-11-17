<?php

echo "<pre>";

foreach(json_decode(file_get_contents('php://input'), true) as $key => $value){
	$_POST[$key] = $value; 
}
echo "<br/><br/>";
print_r($_POST);

echo "<br/><br/>";
echo $_POST["favorites"]["sport"];

echo "<br/><br/>";

if($_POST["registered"]){
	echo "this person is registered";
}

echo "<br/><br/>";

print_r($_GET);

echo "</pre>";

?>
