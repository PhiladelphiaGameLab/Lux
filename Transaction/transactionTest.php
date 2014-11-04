<?php
	include 'Transaction.php';

	function testCompleteQueryString() {
		$q = array(
			"method" => "transaction",		
			"id0" => 111,
			"subId0" => 333,
			"subId1" => 444,
			"itemsId1" => [777, 888]);
		assert(completeQueryString($q) == false);
		$q["id1"] = 222;
		assert(completeQueryString($q) == false);
		$q["itemsId0"] = [555, 666];
		assert(completeQueryString($q) == true);
		$q = array(
			"method" => "transaction",		
			"id1" => "222",
			"itemsId0" => ["555", "666"],
			"itemsId1" => ["777", "888"]);
		assert(completeQueryString($q) == false);
		$q["subId0"] = "333";
		assert(completeQueryString($q) == false);
		$q["subId1"] = "444";
		assert(completeQueryString($q) == false);
		$q["id0"] = "111";
		assert(completeQueryString($q) == true);
	}

	function testCompareItemArrays() {
		$i0 = array("a", "b");
		$i1 = array("b", "c", "d", "a");
		assert(compareItemArrays($i0, $i1) == false);
		assert(compareItemArrays($i1, $i0) == false);
		$i0[2] = "c";
		assert(compareItemArrays($i0, $i1) == false);
		assert(compareItemArrays($i1, $i0) == false);		
		$i0[3] = "d";
		assert(compareItemArrays($i0, $i1) == true);
		assert(compareItemArrays($i1, $i0) == true);
		$i1[1] = "e";
		assert(compareItemArrays($i0, $i1) == false);
		assert(compareItemArrays($i1, $i0) == false);
		$i0[2] = "e";
		assert(compareItemArrays($i0, $i1) == true);
		assert(compareItemArrays($i1, $i0) == true);
	}

	function testCompareTransactions() {
		$q0 = array(
			"method" => "transaction",		
			"id0" => "111",
			"id1" => "222",
			"subId0" => "333",
			"subId1" => "444",
			"itemsId0" => ["666", "555"],
			"itemsId1" => ["777", "888"]);
		$q1 = array(
			"method" => "transaction",		
			"id0" => "222",
			"id1" => "111",
			"subId0" => "444",
			"subId1" => "333",
			"itemsId0" => ["888", "777"],
			"itemsId1" => ["555", "666"]);

		assert(compareTransactions($q0, $q1) == true);
		assert(compareTransactions($q1, $q0) == true);
		$q0["id0"] = "112";
		assert(compareTransactions($q0, $q1) == false);
		assert(compareTransactions($q1, $q0) == false);
		$q1["id1"] = "112";
		assert(compareTransactions($q0, $q1) == true);
		assert(compareTransactions($q1, $q0) == true);
		$q1["subId0"] = "454";
		assert(compareTransactions($q0, $q1) == false);
		assert(compareTransactions($q1, $q0) == false);
		$q0["subId1"] = "454";
		assert(compareTransactions($q0, $q1) == true);
		assert(compareTransactions($q1, $q0) == true);
		$q0["itemsId1"][2] = "999";
		assert(compareTransactions($q0, $q1) == false);
		assert(compareTransactions($q1, $q0) == false);
		$q1["itemsId0"] = ["777", "999", "888"];
		assert(compareTransactions($q0, $q1) == true);
		assert(compareTransactions($q1, $q0) == true);
	}

	function testGetPartyDetails() {
		$q = array(
			"method" => "transaction",		
			"id0" => "111",
			"id1" => "222",
			"subId0" => "333",
			"subId1" => "444",
			"itemsId0" => ["666", "555"],
			"itemsId1" => ["777", "888"]);
		$p0 = getPartyDetails($q, 0);
		assert($p0["id"] == "111");
		assert($p0["sub"] == "333");
		assert($p0["items"][0] == "666");
		assert($p0["items"][1] == "555");
		$p1 = getPartyDetails($q, 1);
		assert($p1["id"] == "222");
		assert($p1["sub"] == "444");
		assert($p1["items"][0] == "777");
		assert($p1["items"][1] == "888");
	}

	testCompleteQueryString();
	testCompareItemArrays();
	testCompareTransactions();
	testGetPartyDetails();

	// function makeRequest($query) {
	// 	$trans_server = "http://localhost/ts/transactionServer.php?";
	//  $query = array(...);
	// 	$query_string = http_build_query($query);
	// 	$request = $trans_server . $query_string;
	// 	return file_get_contents($request);
	// }
?>