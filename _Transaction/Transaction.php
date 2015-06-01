<?php

	/**
	 * Creates a query array from request query string.
	 */
	function createTransactionQuery($id) {
		$query_str = $_SERVER['QUERY_STRING'];
		parse_str($query_str, $query);
		unset($query["acc_token"]);
		$query["id0"] = $id;
		unset($query["method"]);
		if(isset($query["testing"])) {
			unset($query["testing"]);
		}
		if(!completeQueryString($query)) {
			return NULL;
		}
		return $query;
	}

	/**
	 * Returns whether the given array contains all required
	 * information: id0, id1, subId0, subId1, itemsId0, and itemsId1.
	 */
	function completeQueryString($query) {
		if(isset($query["itemsId0"]) && !is_array($query["itemsId0"])) return false;
		if(isset($query["itemsId1"]) && !is_array($query["itemsId1"])) return false;
		return !empty($query["id0"]) && !empty($query["id1"]) &&
				!empty($query["subId0"]) && !empty($query["subId1"]) &&
				(isset($query["itemsId0"]) && is_array($query["itemsId0"]) || 
					isset($query["itemsId1"]) && is_array($query["itemsId1"]));
	}

	/**
	 * Compares two transaction queries. The fields should be mirrored,
	 * so id0 of $query0 should correspond to id1 of $query1.
	 */
	function matchTransactions($query0, $query1) {
		$query0items0 = null;
		if(isset($query0["itemsId0"]) && is_array($query0["itemsId0"])) $query0items0 = $query0["itemsId0"];
		$query0items1 = null;
		if(isset($query0["itemsId1"]) && is_array($query0["itemsId1"])) $query0items1 = $query0["itemsId1"];
		$query1items0 = null;
		if(isset($query1["itemsId0"]) && is_array($query1["itemsId0"])) $query1items0 = $query1["itemsId0"];
		$query1items1 = null;
		if(isset($query1["itemsId1"]) && is_array($query1["itemsId1"])) $query1items1 = $query1["itemsId1"];

		return strcmp($query0["id0"], $query1["id1"]) == 0 &&
			strcmp($query0["id1"], $query1["id0"]) == 0 &&
			strcmp($query0["subId0"], $query1["subId1"]) == 0 &&
			strcmp($query0["subId1"], $query1["subId0"]) == 0 &&
			compareItemArrays($query0items0, $query1items1) &&
			compareItemArrays($query0items1, $query1items0);		
	}

	function flipTransaction($transaction) {
		$flipped = array(		
			"id0" => $transaction["id1"],
			"id1" => $transaction["id0"],
			"subId0" => $transaction["subId1"],
			"subId1" => $transaction["subId0"],
			"type" => $transaction["type"],
			"type_code" => $transaction["type_code"],
			"date" => $transaction["date"]);
		if(isset($transaction["itemsId1"])) {
			$flipped["itemsId0"] = $transaction["itemsId1"];
		}
		if(isset($transaction["itemsId0"])) {
			$flipped["itemsId1"] = $transaction["itemsId0"];
		}
		return $flipped;
	}

	/**
	 * Returns whether two arrays of items are identical.
	 */
	function compareItemArrays($items0, $items1) {
		if($items0 == null && $items1 ==  null) {
			return true;
		} else if($items0 == null || $items1 ==  null) {
			return false;
		} else {
			return empty(array_diff($items0, $items1)) && empty(array_diff($items1, $items0));
		}
	}

	/**
	 * Printing for testing
	 */
	function printQuery($query) {
		if(is_array($query)) {
			foreach ($query as $key => $value) {
	   			if(is_array($value)) {
	   				echo $key . ": ";
	   				for($i = 0; $i < count($value) - 1; $i++) {
	  					echo $value[$i]. ", ";
	  				}
	  				echo $value[count($value) - 1]. "<br>";
	    		} else {
	        		 echo $key . ": " . $value . "<br>";
	    		}
			}
		}
	}

?>  
