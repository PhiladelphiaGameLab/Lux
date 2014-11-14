<?php

	include 'Transaction.php';
	include 'MockOutput.php';
	include 'MockAuth.php';
	//include '/core/output.php'; //When output implemented
	//include '/core/auth.php'; //When authorization implemented

	$TYPE_ACCOUNT = 1;
	$TYPE_SUBACCOUNT = 2;
	$TYPE_GLOBAL_ACCOUNT = 4;
	$TYPE_GROUP_ACCOUNT = 8;
	$USER_ROOT = "user_root";
	$GLOBAL_ROOT = "global_root";
	$GROUP_ROOT = "group_root";
	$TRANSACTION_ROOT = "tree_root";
	$ROOT_TYPE = "root_type";
	$output = new MockOutput();
	$auth = new MockAuth();
	$collection = findCollection();
	$euid = $auth->getClientId();

	if(NULL == $euid) {
		$output->failure("Invalid acc_token");
		return;
	}
	if(!isset($_GET["method"])) {
		$output->failure("No method specified");
		return;
	}

	switch ($_GET["method"]) {

		case "initTreeStruct":
			if (NULL == $collection->findOne() || forceInit()) {
				initTreeStruct();
			} else {
				$output->failure("Collection not reinitialized. Enter query " . 
					"'force=true' to override.");
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
			$output->failure("Requested method does not exist.");
	}

	/**
	 * Removes any existing documents and sets up hierarchical account branches: a tree root with 
	 * references to the user, global, and group roots.
	 */
	function initTreeStruct() {
		global $collection, $USER_ROOT, $GLOBAL_ROOT, $GROUP_ROOT, $TRANSACTION_ROOT, $ROOT_TYPE;
		$collection->remove();
		$users = array($ROOT_TYPE => $USER_ROOT, "accounts" => array());	
		$globals = array($ROOT_TYPE => $GLOBAL_ROOT, "accounts" => array());	
		$groups = array($ROOT_TYPE => $GROUP_ROOT, "accounts" => array());
		$collection->insert($users);
		$collection->insert($globals);
		$collection->insert($groups);
		$tree = array($ROOT_TYPE => $TRANSACTION_ROOT, 
			$USER_ROOT => $users['_id']->{'$id'},
			$GLOBAL_ROOT => $globals['_id']->{'$id'},
			$GROUP_ROOT => $groups['_id']->{'$id'});
		$collection->insert($tree);

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
		global $euid, $collection, $output, $TYPE_ACCOUNT, $TYPE_GLOBAL_ACCOUNT;
		$query = array("euid" => $euid);
		if (NULL == $collection->findOne($query)) {
			$account = array("euid" => $euid, "type" => $TYPE_ACCOUNT,
				"subaccounts" => array(), "groups" => array(), 
				"transactions" => array());
			$collection->insert($account);
			$output->success("Added account");
		} else {
			$output->failure("Account already exists");
		}
	}

	/**
	 * Removes account record for given $euid if it exists.
	 */
	function removeAccount() {
		global $euid, $collection, $output;
		$query = array("euid" => $euid);
		if (NULL != $collection->findOne($query)) {
			$collection->remove($query);
			$output->success("Removed account");
		} else {
			$output->failure("Could not find account");
		}	
	}

	/**
	 * Adds a new subaccount for the account with the given $euid.
	 * It creates a subaccount record and adds its _id to the account's
	 * subaccount array.
	 */
	function addSubaccount() {
		global $euid, $collection, $output, $TYPE_SUBACCOUNT;
		$query = array("euid" => $euid);
		if (NULL != $collection->findOne($query)) {
			$subaccount = array("type" => $TYPE_SUBACCOUNT, "parent" => $euid,
				"items" => array(), "pending" => array());
			$collection->insert($subaccount);
			$collection->update($query, 
 				array('$push' => array("subaccounts" => $subaccount['_id']->{'$id'}))
			);
			$output->success("Added subaccount");
		} else {
			$output->failure("Could not find account");
		}
	}

	/**
	 * Removes the given subaccount record and its reference in the subaccounts
	 * array of its parent account.
	 */
	function removeSubaccount() {
		global $euid, $collection, $output, $TYPE_SUBACCOUNT;
		if(!isset($_GET["subId0"])) {
			$output->failure("No subaccount specified");
			return;
		}
		$subId = $_GET["subId0"];
		$queryEuid = array("euid" => $euid);
		$querySubaccount = array("_id" => new MongoID($subId));
		if (NULL == $collection->findOne($queryEuid)) {
			$output->failure("Could not find account");
		} elseif (NULL == $collection->findOne($querySubaccount)) {
			$output->failure("Could not find subaccount");
		} else {
			$collection->update($queryEuid, array('$pull' => array("subaccounts" => $subId)));
			$collection->remove($querySubaccount);
			$output->success("Removed subaccount");
		}
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

	function setDbName() {
		if(isset($_GET["testing"]) && strcmp($_GET["testing"], "true") == 0) {
			return "LuxTest";
		} else {
			return "Lux";
		}
	}

	function findCollection() {
		$collName = "transaction";
		$mongoServer = "mongodb://54.88.133.189/api/"; //If connecting remotely
		$dbName = setDbName();
		$m = new MongoClient();
		$db = $m->selectDB($dbName);
		return $db->selectCollection($collName);
	}

?>