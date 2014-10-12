<?php

	include 'Transaction.php';

	function makeTransaction() {
		$trans_server = "http://localhost/ts/transactionServer.php?";
		$data = array(
			"method" => "transaction",		
			"id0" => 111,
			"id1" => 222,
			"subId0" => 333,
			"subId1" => 444,
			"itemsId0" => [555, 666],
			"itemsId1" => [777, 888]);
		$query_string = http_build_query($data);
		$request = $trans_server . $query_string;
		return file_get_contents($request);
	}


	echo makeTransaction();

	$data1 = array(
		"method" => "transaction",		
		"id0" => 111,
		"id1" => 222,
		"subId0" => 333,
		"subId1" => 444,
		"itemsId0" => [555, 666],
		"itemsId1" => [777, 888]);

	$data2 = array(
		"method" => "transaction",		
		"id0" => 222,
		"id1" => 111,
		"subId0" => 444,
		"subId1" => 333,
		"itemsId0" => [888, 777],
		"itemsId1" => [555, 666]);

	echo "<br>" . Transaction::compare(Transaction::constructFromArray($data1), 
		Transaction::constructFromArray($data2));
?>