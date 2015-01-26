<?php

	include 'Transaction.php';
	include 'MockOutput.php';
	include 'MockAuth.php';
	//include '/core/output.php'; //When output implemented
	//include '/core/auth.php'; //When authorization implemented

	$TYPE_USER_ACCOUNT = 1;
	$TYPE_SUBACCOUNT = 2;
	$TYPE_GLOBAL_ACCOUNT = 3;
	$TYPE_GROUP_ACCOUNT = 4;
	$TYPE_TRANSACTION = 5;
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
			} elseif (NULL == $collection->findOne() || 
				(isset($_GET["force"]) && $_GET["force"] === "true")) {
				initTreeStruct();
			} else {
				$output->failure("Collection not reinitialized. Enter query " . 
					"'force=true' to override.");
			}
			break;
		case "transaction":
			$transaction = createTransactionQuery($id);
			if(NULL == $transaction) {
				$output->failure("Incomplete transaction details");
				return;
			}
			if(!findTransactionAccounts($transaction)) {
				return;
			}
			requestTransaction($transaction);
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
			if(!isset($_GET["subId"])) {
				$output->failure("No subaccount specified");
				return;
			}
			removeSubaccount();
			break;
		// case "createGroup":
		// 	createGroup();
		// 	break;
		// case "destroyGroup":
		// 	destroyGroup();
		// 	break;
		// case "addGroupMember":
		// 	addGroupMember();
		// 	break;
		// case "removeGroupMember":
		// 	removeGroupMember();
		// 	break;
		// case "changeGroupPermission":
		// 	changeGroupPermission();
		// 	break;
		case "getTransactionHistory":
			getTransactionHistory();
			break;
		case "getTransactionDetail":
			if(!isset($_GET["transId"])) {
				$output->failure("No transId specified");
				return;
			}
			getTransactionDetail();
			break;
		case "getAccountInfo":
			getAccountInfo();
			break;
		case "getSubaccountInfo":
			if(!isset($_GET["subId"])) {
				$output->failure("No subaccount specified");
				return;
			}
			getSubaccountInfo();
			break;
		case "addItemToSubaccount":
			if(!isset($_GET["subId"]) ||!isset($_GET["itemId"])) {
				$output->failure("Specify subId and itemId");
				return;
			}
			addItemToSubAccount();
			break;
		case "removeItemFromSubaccount":
			if(!isset($_GET["subId"]) ||!isset($_GET["itemId"])) {
				$output->failure("Specify subId and itemId");
				return;
			}
			removeItemFromSubAccount();
			break;
		case "getTreeRootInfo":
			if(!$auth->isAdmin()) {
				$output->failure("Not authorized.");
			} else {
				getTreeRootInfo();
			}
			break;
		case "getUserRootInfo":
			if(!$auth->isAdmin()) {
				$output->failure("Not authorized.");
			} else {
				getUserRootInfo();
			}
			break;
		case "getGroupRootInfo":
			if(!$auth->isAdmin()) {
				$output->failure("Not authorized.");
			} else {
				getGroupRootInfo();
			}
			break;
		case "getGlobalRootInfo":
			if(!$auth->isAdmin()) {
				$output->failure("Not authorized.");
			} else {
				getGlobalRootInfo();
			}
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
		$output->success("Transaction tree initialized.", NULL, NULL);
	}

	function requestTransaction($transaction0) { 
		global $id, $output, $TYPE_TRANSACTION;
		$transaction0["type"] = $TYPE_TRANSACTION; //Add type for new MongoDB document
		if(!isGlobalAccount($transaction0['id1'])) {
			$transaction1 = findPending($transaction0);
			if(NULL == $transaction1) {
				$transDoc = createPending($transaction0);
				$output->success("Added pending transaction", $transDoc, NULL);
			} else {
				$transDoc = executeTransaction($transaction0, $transaction1);
				$output->success("Executed transaction with user", $transDoc, NULL);
			}
		} else {
			$transDoc = executeTransaction($transaction0, NULL);
			$output->success("Executed transaction with global", $transDoc, NULL);
		}
	}

	function findTransactionAccounts($transaction) {
		global $output;
		$account0 = findAccount($transaction['id0']);
		if(NULL == $account0) {
			$output->failure("Could not find account id0");
			return FALSE;
		}
		if(!accountContainsSubaccount($account0, $transaction['subId0'])) {
			$output->failure("Could not find subId0 for id0");
			return FALSE;
		}
		$subaccount0 = findSubaccount($transaction['subId0']);
		if(NULL == $subaccount0) {
			$output->failure("Could not find subId0 document");
			return FALSE;
		}
		if(isset($transaction['itemsId0'])) {
			foreach ($transaction['itemsId0'] as $item) {
				if(!in_array($item, $subaccount0["items"])) {
					$output->failure("Could not find item " . $item . " in subId0");
					return FALSE;
				}
			}	
		}
		$account1 = findAccount($transaction['id1']);
		if(NULL == $account1) {
			$output->failure("Could not find account id1");
			return FALSE;
		}
		if(!accountContainsSubaccount($account1, $transaction['subId1'])) {
			$output->failure("Could not find subId1 for id1");
			return FALSE;
		}
		$subaccount1 = findSubaccount($transaction['subId1']);
		if(NULL == $subaccount1) {
			$output->failure("Could not find subId1 document");
			return FALSE;
		}
		if(isset($transaction['itemsId1'])) {
			foreach ($transaction['itemsId1'] as $item) {
				if(!in_array($item, $subaccount1["items"])) {
					$output->failure("Could not find item " . $item . " in subId1");
					return FALSE;
				}
			}	
		}
		return TRUE;
	}

	/**
	 * Returns whether the specified account is a global account.
	 */
	function isGlobalAccount($acct_id) {	//should this take in acct_id or owner id???
		global $collection, $TYPE_GLOBAL_ACCOUNT;
		$query = array('owner_id' => $acct_id);
		$account = $collection->findOne($query);
		return (NULL != $account && (int)$account["type"] == $TYPE_GLOBAL_ACCOUNT); 
	}

	function findPending($transaction) {
		global $collection;
		$subaccount1 = findSubaccount($transaction['subId1']);
		foreach ($subaccount1['pending'] as $value) {		//add cleanup function call here for outdated requests?
			$queryTrans = array('_id' => new MongoID($value));
			$transaction1 = $collection->findOne($queryTrans);			
			if(NULL != $transaction1 && matchTransactions($transaction, $transaction1)) {
				return $transaction1;
			}
		}
		return NULL;
	}

	function createPending($transaction) {
		global $collection;
		$collection->insert($transaction);
		$querySubId = array('_id' => new MongoID($transaction['subId0']));
		$collection->update($querySubId, 
 			array('$push' => array("pending" => $transaction['_id']->{'$id'}))
		);
		return $transaction['_id']->{'$id'};
	}

	function addTransactionToGlobal($transaction0) {
		global $collection;
		$transaction1 = flipTransaction($transaction0);
		$collection->insert($transaction1);
		$queryId1 = array('owner_id' => $transaction0['id1']);
		$collection->update($queryId1, array('$push' => array("transactions" => $transaction1['_id']->{'$id'})));
	}

	function executeTransaction($transaction, $pending) {
		global $collection;
		$querySubId0 = array('_id' => new MongoID($transaction['subId0']));
		$querySubId1 = array('_id' => new MongoID($transaction['subId1']));
		if(isset($transaction['itemsId0']) && is_array($transaction['itemsId0'])) {
			foreach ($transaction['itemsId0'] as $item) {
				$collection->update($querySubId0, array('$pull' => array("items" => $item)));
				$collection->update($querySubId1, array('$push' => array("items" => $item)));
			}
		}
		if(isset($transaction['itemsId1']) && is_array($transaction['itemsId1'])) {
			foreach ($transaction['itemsId1'] as $item) {
				$collection->update($querySubId1, array('$pull' => array("items" => $item)));
				$collection->update($querySubId0, array('$push' => array("items" => $item)));
			}
		}
		if($pending == NULL) {
			addTransactionToGlobal($transaction);
		} else {
			$collection->update($querySubId1, array('$pull' => array("pending" => $pending['_id']->{'$id'})));
			$queryId1 = array('owner_id' => $transaction['id1']);
			$collection->update($queryId1, array('$push' => array("transactions" => $pending['_id']->{'$id'})));
		}	
		$queryId0 = array('owner_id' => $transaction['id0']);	
		$collection->insert($transaction);
		$collection->update($queryId0, array('$push' => array("transactions" => $transaction['_id']->{'$id'})));
		return $transaction['_id']->{'$id'};		
	}

	/**
	 * Creates a new account document of the given type for an authorized owner. 
	 */
	function addAccountDocument($type, $root_name) {
		global $id, $collection, $output;
		if (NULL == findAccount($id)) {
			$account = array("owner_id" => $id, "type" => $type, "subaccounts" => array(), 
				"groups" => array(), "transactions" => array());
			$collection->insert($account);
			updateTreeRoot(true, $root_name, $account['_id']->{'$id'});
			$output->success("Added account", $account['_id']->{'$id'}, NULL);
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
			$output->success("Removed account", NULL, $account['_id']->{'$id'});
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
			$output->success("Added subaccount", $subaccount['_id']->{'$id'}, NULL);
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

		
		if (NULL == findAccount($id)) {
			$output->failure("Could not find account");
			return;
		} 
		$subId = $_GET["subId"];
		$subaccount = findSubaccount($subId);
		if (NULL == $subaccount || $subaccount["parent"] != $id) {
			$output->failure("Could not find subaccount");
		} else {
			$queryId = array("owner_id" => $id);
			$querySubaccount = array('_id' => new MongoID($subId));
			$collection->update($queryId, array('$pull' => array("subaccounts" => $subId)));
			$collection->remove($querySubaccount);
			$output->success("Removed subaccount", NULL, $subId); //orphaned subaccount contents??
		}
	}

	// function createGroup() {

	// }

	// function destroyGroup() {
		
	// }

	// function addGroupMember() {
		
	// }

	// function removeGroupMember() {
		
	// }

	// function changeGroupPermission() {

	// }

	function getTransactionHistory() {
		global $id, $output;
		$accountDoc = findAccount($id); 
		if($accountDoc == NULL) {
			$output->failure("Could not find account");	
			return;		
		}
		$transactions = array("transactions" => $accountDoc["transactions"]);
		$output->success($transactions, NULL, NULL);	
	}

	function getTransactionDetail() { //TODO-----------------------------------------------------------------------------------------
		
	}

	function getAccountInfo() {
		global $id, $output;
		$accountDoc = findAccount($id); 
		if($accountDoc == NULL) {
			$output->failure("Could not find account");	
			return;		
		}
		$output->success($accountDoc, NULL, NULL);
	}

	function verifyAccountAndSubaccount($id, $subId) {
		global $output;
		$accountDoc = findAccount($id); 
		if($accountDoc == NULL) {
			$output->failure("Could not find account");
			return FALSE;
		}
		if(!findSubaccount($subId)) {
			$output->failure("Could not find subaccount");
			return FALSE;
		}
		if(!accountContainsSubaccount($accountDoc, $subId)) {
			$output->failure("Could not find subId for id");
			return FALSE;
		}
		return TRUE;
	}

	function getSubaccountInfo() {
		global $id, $output;
		if(!verifyAccountAndSubaccount($id, $_GET["subId"])) {
			return;
		}
		$sub = findSubaccount($_GET["subId"]);
		echo json_encode($sub["items"]);
		$output->success(findSubaccount($_GET["subId"]), NULL, NULL);
	}

	function addItemToSubAccount() { //Check isAdmin?
		global $id, $collection, $output;
		if(!verifyAccountAndSubaccount($id, $_GET["subId"])) {
			return;
		}
		$subId = $_GET["subId"];
		$item = $_GET["itemId"];
		$querySubId = array('_id' => new MongoID($subId));
		$collection->update($querySubId, array('$push' => array("items" => $item)));
		$output->success("Added item to subaccount", $item, NULL);		
	}

	function removeItemFromSubAccount() {
		global $id, $collection, $output;
		if(!verifyAccountAndSubaccount($id, $_GET["subId"])) {
			return;
		}
		$subId = $_GET["subId"];
		$item = $_GET["itemId"];
		$querySubId = array('_id' => new MongoID($subId), 'items' => $item);
		if(NULL == $collection->findOne($querySubId)) {
			$output->failure("No item itemId in subId");
			return;			
		}
		$collection->update($querySubId, array('$pull' => array("items" => $item)));
		$output->success("Removed item from subaccount", NULL, $item);		
	}

	function getTreeRootInfo() {
		global $ROOT_TYPE, $TRANSACTION_ROOT, $collection, $output;
		$query = array($ROOT_TYPE => $TRANSACTION_ROOT);
		$root = $collection->findOne($query);
		if($root == NULL) {
			$output->failure("Tree root not found");
			return;			
		}
		$output->success($root, NULL, NULL);		
	}

	function getUserRootInfo() {
		global $ROOT_TYPE, $USER_ROOT, $collection, $output;
		$query = array($ROOT_TYPE => $USER_ROOT);
		$root = $collection->findOne($query);
		if($root == NULL) {
			$output->failure("User root not found");
			return;			
		}
		$output->success($root, NULL, NULL);			
	}

	function getGroupRootInfo() {
		global $ROOT_TYPE, $GROUP_ROOT, $collection, $output;
		$query = array($ROOT_TYPE => $GROUP_ROOT);
		$root = $collection->findOne($query);
		if($root == NULL) {
			$output->failure("Group root not found");
			return;			
		}
		$output->success($root, NULL, NULL);			
	}

	function getGlobalRootInfo() {
		global $ROOT_TYPE, $GLOBAL_ROOT, $collection, $output;
		$query = array($ROOT_TYPE => $GLOBAL_ROOT);
		$root = $collection->findOne($query);
		if($root == NULL) {
			$output->failure("Global root not found");
			return;			
		}
		$output->success($root, NULL, NULL);
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

	function findAccount($owner_id) {
		global $collection;
		$query = array('owner_id' => $owner_id);
		return $collection->findOne($query);
	}

	function findUserAccount($owner_id) {
		global $TYPE_USER_ACCOUNT;
		$account = findAccount($owner_id);
		if(NULL == $account || $account["type"] != $TYPE_USER_ACCOUNT) {
			return NULL;
		}
		return $account;
	}

	function findGlobalAccount($ownerId) {
		global $TYPE_GLOBAL_ACCOUNT;
		$account = findAccount($ownerId);
		if(NULL == $account || $account["type"] != $TYPE_GLOBAL_ACCOUNT) {
			return NULL;
		}
		return $account;
	}

	function accountContainsSubaccount($accountDoc, $subId) {
		foreach ($accountDoc['subaccounts'] as $value) {
			if($value == $subId) return TRUE;
		}
		return FALSE;
	}

	function findSubaccount($subId) {
		global $TYPE_SUBACCOUNT, $collection;
		try {
			$querySubId = array('_id' => new MongoID($subId));
		} catch (MongoException $m) {
			return NULL;
		}
		$subaccount = $collection->findOne($querySubId);
		if(NULL == $subaccount || $subaccount['type'] != $TYPE_SUBACCOUNT) {
			return NULL;
		}
		return $subaccount;
	} 

	function setDbName() {
		if(isset($_GET["testing"]) && $_GET["testing"] === "true") {
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