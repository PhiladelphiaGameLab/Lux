<?php

	/**
	 * Creates a query array from request query string.
	 */
	function createTransactionQuery($id) {
		$query_str = $_SERVER['QUERY_STRING'];
		parse_str($query_str, $query);
		$query["id0"] = $id;
		if(!completeQueryString($query)) {
			echo "Incomplete transaction details. Query string
			must include id0, id1, subId0, subId1, itemsId0, 
			and itemsId1.";
			return NULL;
		}
		return $query;
	}

	/**
	 * Returns whether the given array contains all required
	 * information: id0, id1, subId0, subId1, itemsId0, and itemsId1.
	 */
	function completeQueryString($query) {
		$complete = !empty($query["id0"]) && !empty($query["id1"]) &&
					!empty($query["subId0"]) && !empty($query["subId1"]) &&
					!empty($query["itemsId0"]) && !empty($query["itemsId1"]);
		if(!$complete) {
			echo "Incomplete transaction details. Query string
			must include id0, id1, subId0, subId1, itemsId0, 
			and itemsId1.";
		}
		return $complete;
	}

	/**
	 * Compares two transaction queries. The fields should be mirrored,
	 * so id0 of $query0 should correspond to id1 of $query1.
	 */
	function compareTransactions($query0, $query1) {
		return strcmp($query0["id0"], $query1["id1"]) == 0 &&
			strcmp($query0["id1"], $query1["id0"]) == 0 &&
			strcmp($query0["subId0"], $query1["subId1"]) == 0 &&
			strcmp($query0["subId1"], $query1["subId0"]) == 0 &&
			compareItemArrays($query0["itemsId0"], $query1["itemsId1"]) &&
			compareItemArrays($query0["itemsId1"], $query1["itemsId0"]);		
	}

	/**
	 * Returns whether two arrays of items are identical.
	 */
	function compareItemArrays($items0, $items1) {
		return empty(array_diff($items0, $items1)) &&
			empty(array_diff($items1, $items0));
	}


	/**
	 * Yields the id, subaccount id, and items for the given party.
	 * Parameter 0 for id0 and 1 for id1.  
	 */
	function getPartyDetails($query, $party) {
		if($party) {
			return array(
				"id" => $query["id1"],
				"sub" => $query["subId1"],
				"items" => $query["itemsId1"]);
		} else {
			return array(
				"id" => $query["id0"],
				"sub" => $query["subId0"],
				"items" => $query["itemsId0"]);
		}
	}	

	/**
	 * Printing for testing
	 */
	function printQuery($query) {
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

?>