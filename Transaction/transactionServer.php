<?php

	include 'Transaction.php';
	include 'MockOutput.php';
	include 'MockAuth.php';
	//include '/core/output.php'; //When output implemented
	//include '/core/auth.php'; //When authorization implemented

	$TYPE_USER_ACCOUNT = 1;
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
	$id = $auth->getClientId();

	if(NULL == $id) {
		$output->failure("Invalid acc_token");
		return;
	}
	if(!isset($_GET["method"])) {
		$output->failure("No method specified");
		return;
	}

	$method = $_GET["method"];
	if($method != "initTreeStruct" && !treeInitialized()) {
		$output->failure("Tree not initialized");
		return;
	}

	switch ($method) {

		case "initTreeStruct":
			if(!$auth->isAdmin()) {
				$output->failure("Init not authorized.");
			}
			elseif (NULL == $collection->findOne() || forceInit()) {
				initTreeStruct();
			} else {
				$output->failure("Collection not reinitialized. Enter query " . 
					"'force=true' to override.");
			}
			break;
		case "transaction":
			requestTransaction();
			break;
		case "addUserAccount":
			addUserAccount();
			break;
		case "removeUserAccount":
			removeUserAccount();
			break;
		case "addGlobalAccount":
			addGlobalAccount();
			break;
		case "removeGlobalAccount":
			removeGlobalAccount();
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
		global $collection, $USER_ROOT, $GLOBAL_ROOT, $GROUP_ROOT, 
		$TRANSACTION_ROOT, $ROOT_TYPE, $output;
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
		$output->success("Transaction tree initialized.");
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

	//TODO----------------------------------------------------------------------------------------------------------------
	function requestTransaction() { 
		global $id;
		$query = createTransactionQuery($id);
		printQuery($query);
	}

	/**
	 * Returns whether the specified account is a global account.
	 */
	function isGlobalAccount($acct_id) {
		global $collection, $TYPE_GLOBAL_ACCOUNT;
		$query = array('_id' => new MongoID($acct_id));
		$account = $collection->findOne($query);
		return (NULL != $account && (int)$account["type"] == $TYPE_GLOBAL_ACCOUNT); 
	}

	//TODO----------------------------------------------------------------------------------------------------------------
	function searchPending($transaction) {
		return /*pending exists*/;
	}

	/**
	 * Creates a new account document of the given type for an authorized owner. 
	 */
	function addAccountDocument($type, $root_name) {
		global $id, $collection, $output;
		$query = array("owner_id" => $id);
		if (NULL == $collection->findOne($query)) {
			$account = array("owner_id" => $id, "type" => $type,
				"subaccounts" => array(), "groups" => array(), 
				"transactions" => array());
			$collection->insert($account);
			updateTreeRoot(true, $root_name, $account['_id']->{'$id'});
			$output->success("Added account: " . $account['_id']);
		} else {
			$output->failure("Account already exists");
		}
	}

	/**
	 * Adds user account record for given id if one doesn't 
	 * already exist.
	 */
	function addUserAccount() {
		global $TYPE_USER_ACCOUNT, $USER_ROOT;
		addAccountDocument($TYPE_USER_ACCOUNT, $USER_ROOT);
	}

	/**
	 * Adds global account record for given admin id if one doesn't 
	 * already exist.
	 */
	function addGlobalAccount() {
		global $auth, $TYPE_GLOBAL_ACCOUNT, $output, $GLOBAL_ROOT;
		if($auth->isAdmin()) {
			addAccountDocument($TYPE_GLOBAL_ACCOUNT, $GLOBAL_ROOT);
		} else {
			$output->failure("Global account creation not authorized.");
		}
	}

	/**
	 * Removes account record for given euid if it exists.
	 */
	function removeAccountDocument($type, $root_name) {
		global $id, $collection, $output;
		$query = array("owner_id" => $id);
		$account = $collection->findOne($query);
		if (NULL != $account && (int)$account["type"] == $type) {
			updateTreeRoot(false, $root_name, $account['_id']->{'$id'});
			$collection->remove($query);
			$output->success("Removed account");
		} else {
			$output->failure("Could not find account");
		}	
	}

	/**
	 * Removes account for given user if it exists.
	 */
	function removeUserAccount() {
		global $TYPE_USER_ACCOUNT, $USER_ROOT;
		removeAccountDocument($TYPE_USER_ACCOUNT, $USER_ROOT);
	}

	/**
	 * Removes given global account if it exists.
	 */
	function removeGlobalAccount() {
		global $TYPE_GLOBAL_ACCOUNT, $GLOBAL_ROOT;
		removeAccountDocument($TYPE_GLOBAL_ACCOUNT, $GLOBAL_ROOT);
	}

	/**
	 * Adds a new subaccount for the account with the given euid.
	 * It creates a subaccount record and adds its _id to the account's
	 * subaccount array.
	 */
	function addSubaccount() {
		global $id, $collection, $output, $TYPE_SUBACCOUNT;
		$query = array("owner_id" => $id);
		if (NULL != $collection->findOne($query)) {
			$subaccount = array("type" => $TYPE_SUBACCOUNT, "parent" => $id,
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
		global $id, $collection, $output, $TYPE_SUBACCOUNT;
		if(!isset($_GET["subId0"])) {
			$output->failure("No subaccount specified");
			return;
		}
		$subId = $_GET["subId0"];
		$queryId = array("owner_id" => $id);
		$querySubaccount = array('_id' => new MongoID($subId));
		if (NULL == $collection->findOne($queryId)) {
			$output->failure("Could not find account");
		} elseif (NULL == $collection->findOne($querySubaccount)) {
			$output->failure("Could not find subaccount");
		} else {
			$collection->update($queryId, array('$pull' => array("subaccounts" => $subId)));
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

	function updateTreeRoot($add, $root_name, $document) {
		global $ROOT_TYPE, $collection;
		$root_query = array($ROOT_TYPE => $root_name);
		if($add) {	
			$collection->update($root_query, 
 				array('$push' => array("accounts" => $document)));
		} else {
			$collection->update($root_query, 
				array('$pull' => array("accounts" => $document)));
		}
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

	function treeInitialized() {
		global $collection, $ROOT_TYPE, $USER_ROOT, $GLOBAL_ROOT, $GROUP_ROOT ;
		$user = $collection->findOne(array($ROOT_TYPE => $USER_ROOT));
		$global = $collection->findOne(array($ROOT_TYPE => $GLOBAL_ROOT));
		$group = $collection->findOne(array($ROOT_TYPE => $GROUP_ROOT));
		return $user != NULL && $global != NULL && $group != NULL;
	}

?>