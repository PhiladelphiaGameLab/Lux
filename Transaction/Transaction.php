<?php

	/**
     * Defines the details of a request for a transaction.
     * The request comes from the party with id0 for a
     * transaction with party id1.
	 */
	class Transaction {
		private $id0;
		private $id1;
		private $subId0;
		private $subId1;
		private $itemsId0;
		private $itemsId1;

		/**
		 * Constructs Transaction from request query string.
		 */
		public static function constructFromQuery() {
			$instance = new self();
			if(!Transaction::completeQueryString()) {			
				return;
			}
			$instance->id0 = $_GET["id0"];
			$instance->id1 = $_GET["id1"];
			$instance->subId0 = $_GET["subId0"];
			$instance->subId1 = $_GET["subId1"];
			$instance->itemsId0 = $_GET["itemsId0"];
			$instance->itemsId1 = $_GET["itemsId1"];
			return $instance;
		}

		/**
		 * Constructs Transaction from given array parameter.
		 */
		public static function constructFromArray($data) {
			$instance = new self();
			$instance->id0 = $data["id0"];
			$instance->id1 = $data["id1"];
			$instance->subId0 = $data["subId0"];
			$instance->subId1 = $data["subId1"];
			$instance->itemsId0 = $data["itemsId0"];
			$instance->itemsId1 = $data["itemsId1"];
			return $instance;
		}

		/**
		 * Returns whether the http query string contains all required
		 * information: id0, id1, subId0, subId1, itemsId0, and itemsId1.
		 */
		private static function completeQueryString() {
			$complete = isset($_GET["method"]) && isset($_GET["id0"]) && 
				isset($_GET["id1"]) && isset($_GET["subId0"]) && 
				isset($_GET["subId1"]) && isset($_GET["itemsId0"]) && 
				isset($_GET["itemsId1"]);
			if(!$complete) {
				echo "Incomplete transaction details. Query string
				must include id0, id1, subId0, subId1, itemsId0, 
				and itemsId1.";
			}
			return $complete;
		}

		/**
		 * Compares two transactions. The fields should be mirrored,
		 * so $id0 of $trans0 should correspond to $id1 of $trans1.
		 */
		public static function compare($trans0, $trans1) {
			return strcmp($trans0->id0, $trans1->id1) == 0 &&
				strcmp($trans0->id1, $trans1->id0) == 0 &&
				strcmp($trans0->subId0, $trans1->subId1) == 0 &&
				strcmp($trans0->subId1, $trans1->subId0) == 0 &&
				count($trans0->itemsId0) ==  count($trans1->itemsId1) &&
				count($trans0->itemsId1) ==  count($trans1->itemsId0) &&
				Transaction::compareItemArrays($trans0->itemsId0, $trans1->itemsId1) &&
				Transaction::compareItemArrays($trans0->itemsId1, $trans1->itemsId0);		
		}

		/**
		 * Returns whether two arrays of items are identical.
		 */
		private static function compareItemArrays($items0, $items1) {
			sort($items0);
			sort($items1);
			$count = count($items0);
			for($i = 0; $i < $count; $i++) {
				if(strcmp($items0[$i], $items1[$i]) != 0) {
					return false;
				}
			}	
			return true;
		}

		/*
		 * Returns the array of transaction details.
		 */
		public function getDetails() {
			return array(
				"id0" => $this->id0,
				"id1" => $this->id1,
				"subId0" => $this->subId0,
				"subId1" => $this->subId1,
				"itemsId0" => $this->itemsId0,
				"itemsId1" => $this->itemsId1);
		}

		/**
		 * Yields the id, subaccount id, and items for the given party.
		 * Parameter 0 for id0 and 1 for id1.  
		 */
		public function getPartyDetails($party) {
			if($party) {
				return array(
					"id" => $this->id1,
					"sub" => $this->subId1,
					"items" => $this->itemsId1);
			} else {
				return array(
					"id" => $this->id0,
					"sub" => $this->subId0,
					"items" => $this->itemsId0);
			}
		}	

		/**
		 * Prints the transaction details.
		 */
		public function printDetails() {
			echo "<br>id0: " . $this->id0 . "<br>id1: " . 
			$this->id1 . "<br>subId0: " . $this->subId0 . 
			"<br>subId1: " . $this->subId1 . "<br>itemsId0: " . 
			json_encode($this->itemsId0) . "<br>itemsId1: " . 
			json_encode($this->itemsId1);
		}

	}

?>