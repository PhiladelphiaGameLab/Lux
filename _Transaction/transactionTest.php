<?php
	include 'Transaction.php';

	function testCompleteQueryString() {
		$q = array(
			"method" => "transaction",		
			"id0" => 111,
			"subId0" => 333,
			"subId1" => 444);
		assert(completeQueryString($q) == false);
		$q["id1"] = 222;
		assert(completeQueryString($q) == false);
		$q["itemsId0"] = 555;
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
		$q0 = array(
			"method" => "transaction",		
			"id0" => "111",
			"id1" => "222",
			"subId0" => "333",
			"subId1" => "444");
		$q1 = array(
			"method" => "transaction",		
			"id0" => "222",
			"id1" => "111",
			"subId0" => "444",
			"subId1" => "333",
			"itemsId1" => ["b", "c", "d", "a"]);
		assert(matchTransactions($q0, $q1) == false);
		assert(matchTransactions($q1, $q0) == false);		
		$q0["itemsId0"] = ["a", "b"];
		assert(matchTransactions($q0, $q1) == false);
		assert(matchTransactions($q1, $q0) == false);	
		$q0["itemsId0"][2] = "c";
		assert(matchTransactions($q0, $q1) == false);
		assert(matchTransactions($q1, $q0) == false);	
		$q0["itemsId0"][3] = "d";		
		assert(matchTransactions($q0, $q1) == true);
		assert(matchTransactions($q1, $q0) == true);	
		$q1["itemsId1"][1] = "e";	
		assert(matchTransactions($q0, $q1) == false);
		assert(matchTransactions($q1, $q0) == false);	
		$q0["itemsId0"][2] = "e";
		assert(matchTransactions($q0, $q1) == true);
		assert(matchTransactions($q1, $q0) == true);
	}

	function testMatchTransactions() {
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

		assert(matchTransactions($q0, $q1) == true);
		assert(matchTransactions($q1, $q0) == true);
		$q0["id0"] = "112";
		assert(matchTransactions($q0, $q1) == false);
		assert(matchTransactions($q1, $q0) == false);
		$q1["id1"] = "112";
		assert(matchTransactions($q0, $q1) == true);
		assert(matchTransactions($q1, $q0) == true);
		$q1["subId0"] = "454";
		assert(matchTransactions($q0, $q1) == false);
		assert(matchTransactions($q1, $q0) == false);
		$q0["subId1"] = "454";
		assert(matchTransactions($q0, $q1) == true);
		assert(matchTransactions($q1, $q0) == true);
		$q0["itemsId1"][2] = "999";
		assert(matchTransactions($q0, $q1) == false);
		assert(matchTransactions($q1, $q0) == false);
		$q1["itemsId0"] = ["777", "999", "888"];
		assert(matchTransactions($q0, $q1) == true);
		assert(matchTransactions($q1, $q0) == true);
	}

	function testFlipTransaction() {
		$q0 = array(	
			"id0" => "111",
			"id1" => "222",
			"subId0" => "333",
			"subId1" => "444",
			"itemsId0" => ["666", "555"],
			"itemsId1" => ["777", "888"]);
		$q1 = flipTransaction($q0);
		assert($q1["id0"] == "222");
		assert($q1["id1"] == "111");
		assert($q1["subId0"] == "444");
		assert($q1["subId1"] == "333");
		assert($q1["itemsId0"][0] == "777");
		assert($q1["itemsId0"][1] == "888");
		assert($q1["itemsId1"][0] == "666");
		assert($q1["itemsId1"][1] == "555");
	}

	testCompleteQueryString();
	testCompareItemArrays();
	testMatchTransactions();
	testFlipTransaction();

?>  
