<?php
include_once("../../core/lux-functions.php");
include_once("../../core/output.php");
include_once("../../core/db.php");
include_once("../../core/auth.php");


$OUTPUT = new Output();
echo "\n\n";
$OUTPUT->success("I've Won!", array("what" => "this"));

echo "\n\n";
$OUTPUT->addToPrintArray(array("test"=> "ing", "and"=> "more", "test" => "ings"));
$OUTPUT->addToPrintArray(array("test2"=> "ing2", "and2"=> array("even" => "more2", "test2" => "ings2")));
$OUTPUT->addToPrintArray("testing", "some", "more");

$OUTPUT->printArray();

echo "\n\n";
$OUTPUT->error("I've Lost!", array("what" => "this"));
echo "\n\n";

?>
