<?php

	include 'Transaction.php';
	//include '/core/auth.php'; //When authorization class utilized

	// $auth = new Auth();
	// $euid = $auth->getClientId();
	//Temporary auth bypass
	$euid = NULL;
	if(isset($_GET["acc_token"])) {
		$euid = $_GET["acc_token"];
	} else {
		echo "Invalid acc_token";
		return;
	}

	$dbName = "Lux";
	$COLL_NAME = "transaction";
	$MONGO_SERVER = "mongodb://54.88.133.189/api/"; //If connecting remotely
	$TYPE_ACCOUNT = 1;
	$TYPE_SUBACCOUNT = 2;
	$TYPE_GLOBAL_ACCOUNT = 4;
	$TYPE_GROUP_ACCOUNT = 8;
	if(isset($_GET["testing"]) && strcmp($_GET["testing"], "true") == 0) {
		$dbName = "LuxTest";
	}
	$m = new MongoClient();
	$db = $m->selectDB($dbName);
	$collection = $db->selectCollection($COLL_NAME);

	if(!isset($_GET["method"])) {
		echo "No method specified.";
		return;
	}

	switch ($_GET["method"]) {

		case "initTreeStruct":
			if (NULL == $collection->findOne() || forceInit()) {
				initTreeStruct();
			} else {
				echo "Collection not reinitialized. Enter query 
					'force=true' to override.";
			}
			break;
		case "transaction":
			requestTransaction();
			break;
		case "addAccount":
			addAccount();
			break;
		case "removeAccount":
			removeAccount();
			break;
		case "addSubaccount":
			addSubaccount();
			break;
		case "removeSubaccount":
			removeSubaccount();
			break;
		case "createGroup":
			createGroup();
			break;
		case "destroyGroup":
			destroyGroup();
			break;
		case "addGroupMember":
			addGroupMember();
			break;
		case "removeGroupMember":
			removeGroupMember();
			break;
		case "changeGroupPermission":
			changeGroupPermission();
			break;
		case "getTransactionHistory":
			getTransactionHistory();
			break;
		case "getTransactionDetail":
			getTransactionDetail();
			break;
		case "getAccountInfo":
			getAccountInfo();
			break;
		case "getSubaccountInfo":
			getSubaccountInfo();
			break;
		case "addItemToSubAccount":
			addItemToSubAccount();
			break;
		case "removeItemFromSubAccount":
			removeItemFromSubAccount();
			break;
		case "addGlobalAccount":
			addGlobalAccount();
			break;
		case "removeGlobalAccount":
			removeGlobalAccount();
			break;
		case "getTreeRootInfo":
			getTreeRootInfo();
			break;
		case "getUserRootInfo":
			getUserRootInfo();
			break;
		case "getGroupRootInfo":
			getGroupRootInfo();
			break;
		case "getGlobalRootInfo":
			getGlobalRootInfo();
			break;
		case "get":
			get();
			break;
		default:
			echo "Requested method does not exist.";
	}

	/**
	 * TODO: Sets up hierarchical account branches
	 */
	function initTreeStruct() {
		echo "init";
	}

	/**
	 * Checks if query string includes override to remove
	 * existing records and reinitialze hierarchical 
	 * account branches.
	 */
	function forceInit() {
		return isset($_GET["force"]) && 
			strcmp($_GET["force"], "true") == 0;
	}

	function requestTransaction() {
		$query = createTransactionQuery($euid);
		printQuery($query);
	}

	function isGlobalAccount($id) {
		$globalRoot = getGlobalRootInfo();
		return /*$globalRoot members contains $id*/;
	}

	function searchPending($transaction) {
		return /*pending exists*/;
	}

	/**
	 * Adds account record for given $euid if one doesn't 
	 * already exist.
	 */
	function addAccount() {
		global $euid, $collection, $TYPE_ACCOUNT, $TYPE_GLOBAL_ACCOUNT;
		$query = array("euid" => $euid);
		if (NULL == $collection->findOne($query)) {
			$account = array("euid" => $euid, "type" => $TYPE_ACCOUNT,
				"subaccounts" => array(), "groups" => array(), 
				"transactions" => array());
			$collection->insert($account);
			echo "addAccount: success";
		} else {
			echo "addAccount: failure (Account already exists)";
		}
	}

	/**
	 * Removes account record for given $euid if it exists.
	 */
	function removeAccount() {
		global $euid, $collection;
		$query = array("euid" => $euid);
		if (NULL != $collection->findOne($query)) {
			$collection->remove($query);
			echo "removeAccount: success";
		} else {
			echo "removeAccount: failure (Could not find account)";
		}	
	}

	/**
	 * Adds a new subaccount for the account with the given $euid.
	 * It creates a subaccount record and adds its _id to the account's
	 * subaccount array.
	 */
	function addSubaccount() {
		global $euid, $collection, $TYPE_SUBACCOUNT;
		$query = array("euid" => $euid);
		if (NULL != $collection->findOne($query)) {
			$subaccount = array("type" => $TYPE_SUBACCOUNT, "parent" => $euid,
				"items" => array(), "pending" => array());
			$collection->insert($subaccount);
			$collection->update($query, 
 				array('$push' => array("subaccounts" => $subaccount['_id']))
			);
			echo "addSubaccount: success";
		} else {
			echo "addSubaccount: failure (Could not find account)";
		}
	}

	function removeSubaccount() {
		//uses pull function to remove _id from subaccount array, and
		//remove function to remoe subaccount record
	}

	function createGroup() {

	}

	function destroyGroup() {
		
	}

	function addGroupMember() {
		
	}

	function removeGroupMember() {
		
	}

	function changeGroupPermission() {

	}

	function getTransactionHistory() {
		
	}

	function getTransactionDetail() {
		
	}

	function getAccountInfo() {
		
	}

	function getSubaccountInfo() {

	}

	function addItemToSubAccount() {
		
	}

	function removeItemFromSubAccount() {
		
	}

	function addGlobalAccount() {
		
	}

	function removeGlobalAccount() {

	}

	function getTreeRootInfo() {
		
	}

	function getUserRootInfo() {
		
	}

	function getGroupRootInfo() {
		
	}

	function getGlobalRootInfo() {

	}

	function get() {
		
	}

?>