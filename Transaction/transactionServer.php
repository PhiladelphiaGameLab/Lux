<?php

    include 'Transaction.php';
    include 'MockOutput.php';
    include 'MockAuth.php';
    //include '/core/output.php'; //When output implemented
    //include '/core/auth.php'; //When authorization implemented

    define('C', 'CODE');
    define('T', 'TYPE');
    define('USER_ACCT', serialize(array(C => 1, T => 'user_account')));
    define('SUBACCOUNT', serialize(array(C => 2, T => 'subaccount')));
    define('GLOBAL_ACCT', serialize(array(C => 3, T => 'global_account')));
    define('GROUP', serialize(array(C => 4, T => 'group')));
    define('TRANS', serialize(array(C => 5, T => 'transaction')));
    define('PENDING', serialize(array(C => 6, T => 'pending')));
    define('USER_ROOT', serialize(array(C => 7, T => 'user_root')));
    define('GLOBAL_ROOT', serialize(array(C => 8, T => 'global_root')));
    define('GROUP_ROOT', serialize(array(C => 9, T => 'group_root')));
    define('TREE_ROOT', serialize(array(C => 10, T => 'tree_root')));
    define('OUT', serialize(new MockOutput()));

    $collection = findCollection();
    $auth = new MockAuth();
    define('UID', $auth->getClientId());
    define('ADMIN', $auth->isAdmin());

    if(!UID) {
        u(OUT)->failure("Invalid acc_token");
        return;
    }
    if(!isset($_GET["method"])) {
        u(OUT)->failure("No method specified");
        return;
    }

    $method = $_GET["method"];
    if($method != "initTreeStruct" && !treeInitialized()) {
        u(OUT)->failure("Tree not initialized");
        return;
    }

    switch ($method) {

        case "initTreeStruct":
            if(!ADMIN) {
                u(OUT)->failure("Init not authorized.");
            } elseif (!$collection->findOne() || 
                (isset($_GET["force"]) && $_GET["force"] === "true")) {
                initTreeStruct();
            } else u(OUT)->failure("Not reinitialized. Query 'force=true' to override.");
            break;
        case "transaction":
            $transaction = createTransactionQuery(UID);
            if(!$transaction) u(OUT)->failure("Incomplete transaction details");
            elseif(findTransactionAccounts($transaction)) requestTransaction($transaction);
            break;
        case "addUserAccount":
            addAccountDocument(u(USER_ACCT), u(USER_ROOT)[C]);
            break;
        case "removeUserAccount":
            removeAccountDocument(u(USER_ACCT), u(USER_ROOT)[C]);
            break;
        case "addGlobalAccount":
            if(ADMIN) addAccountDocument(u(GLOBAL_ACCT), u(GLOBAL_ROOT)[C]);
            else u(OUT)->failure("Global account creation not authorized.");
            break;
        case "removeGlobalAccount":
            removeAccountDocument(u(GLOBAL_ACCT), u(GLOBAL_ROOT)[C]);
            break;
        case "addSubaccount":
            addSubaccount();
            break;
        case "removeSubaccount":
            if(!isset($_GET["subId"])) u(OUT)->failure("No subaccount specified");
            else removeSubaccount();
            break;
    //  // case "createGroup":
    //  //  createGroup();
    //  //  break;
    //  // case "destroyGroup":
    //  //  destroyGroup();
    //  //  break;
    //  // case "addGroupMember":
    //  //  addGroupMember();
    //  //  break;
    //  // case "removeGroupMember":
    //  //  removeGroupMember();
    //  //  break;
    //  // case "changeGroupPermission":
    //  //  changeGroupPermission();
    //  //  break;
    //  case "getTransactionHistory":
    //      getTransactionHistory();
    //      break;
    //  case "getTransactionDetail":
    //      if(!isset($_GET["transId"])) {
    //          $output->failure("No transId specified");
    //          return;
    //      }
    //      getTransactionDetail();
    //      break;
    //  case "getAccountInfo":
    //      getAccountInfo();
    //      break;
    //  case "getSubaccountInfo":
    //      if(!isset($_GET["subId"])) {
    //          $output->failure("No subaccount specified");
    //          return;
    //      }
    //      getSubaccountInfo();
    //      break;
        case "addItemToSubaccount":
            if(!isset($_GET["subId"]) ||!isset($_GET["itemId"])) {
                u(OUT)->failure("Specify subId and itemId");
                return;
            }
            addItemToSubAccount();
            break;
        case "removeItemFromSubaccount":
            if(!isset($_GET["subId"]) ||!isset($_GET["itemId"])) {
                u(OUT)->failure("Specify subId and itemId");
                return;
            }
            removeItemFromSubAccount();
            break;
    //  case "getTreeRootInfo":
    //      if(!$auth->isAdmin()) {
    //          $output->failure("Not authorized.");
    //      } else {
    //          getTreeRootInfo();
    //      }
    //      break;
    //  case "getUserRootInfo":
    //      if(!$auth->isAdmin()) {
    //          $output->failure("Not authorized.");
    //      } else {
    //          getUserRootInfo();
    //      }
    //      break;
    //  case "getGroupRootInfo":
    //      if(!$auth->isAdmin()) {
    //          $output->failure("Not authorized.");
    //      } else {
    //          getGroupRootInfo();
    //      }
    //      break;
    //  case "getGlobalRootInfo":
    //      if(!$auth->isAdmin()) {
    //          $output->failure("Not authorized.");
    //      } else {
    //          getGlobalRootInfo();
    //      }
    //      break;
        default:
            u(OUT)->failure("Requested method does not exist.");
    }

    /**
     * Removes any existing documents and sets up hierarchical account branches: a tree root with 
     * references to the user, global, and group roots.
     */
    function initTreeStruct() {
        global $collection;
        $collection->remove();
        $users = array("type" => u(USER_ROOT)[T], "type_code" => u(USER_ROOT)[C], "accounts" => array());
        $globals = array("type" => u(GLOBAL_ROOT)[T], "type_code" => u(GLOBAL_ROOT)[C], "accounts" => array()); 
        $groups = array("type" => u(GROUP_ROOT)[T], "type_code" => u(GROUP_ROOT)[C], "accounts" => array());
        $collection->insert($users);
        $collection->insert($globals);
        $collection->insert($groups);
        $tree = array("type" => u(TREE_ROOT)[T], "type_code" => u(TREE_ROOT)[C],
            u(USER_ROOT)[T] => $users['_id']->{'$id'},
            u(GLOBAL_ROOT)[T] => $globals['_id']->{'$id'},
            u(GROUP_ROOT)[T] => $groups['_id']->{'$id'});
        $collection->insert($tree);
        u(OUT)->success("Transaction tree initialized.", NULL, NULL);
    }

    function requestTransaction($transaction0) { 
        if(findAccount($transaction0['id1'], u(USER_ACCT)[C])) { //user account, not global
            $transaction1 = findPending($transaction0);
            if(!$transaction1) {
                $transDoc = createPending($transaction0);
                u(OUT)->success("Added pending transaction", $transDoc, NULL);
            } else {
                $transDoc = executeTransaction($transaction0, $transaction1);
                u(OUT)->success("Executed transaction with user", $transDoc, NULL);
            }
        } else {
            $transDoc = executeTransaction($transaction0, NULL);
            u(OUT)->success("Executed transaction with global", $transDoc, NULL);
        }
    }

    function findTransactionAccounts($t) {
        if(!validAcctAndSubacct($t['id0'], $t['subId0'], "id0", "subId0")) return;
        $account0 = findAccount($t['id0']);
        $subaccount0 = findSubaccount($t['subId0']);
        if(isset($t['itemsId0'])) {         
            foreach ($t['itemsId0'] as $item) {
                if(!in_array($item, $subaccount0["items"])) {
                    u(OUT)->failure("Could not find item " . $item . " in subId0");
                    return;
                }
            }   
        }
        if(!validAcctAndSubacct($t['id1'], $t['subId1'], "id1", "subId1")) return;
        $account1 = findAccount($t['id1']);
        $subaccount1 = findSubaccount($t['subId1']);
        if(isset($t['itemsId1'])) {
            foreach ($t['itemsId1'] as $item) {
                if(!in_array($item, $subaccount1["items"])) {
                    u(OUT)->failure("Could not find item " . $item . " in subId1");
                    return;
                }
            }   
        }
        return TRUE;
    }

    function findPending($transaction) {
        global $collection;
        $subaccount1 = findSubaccount($transaction['subId1']);
        foreach ($subaccount1['pending'] as $value) {       //add cleanup function call here for outdated requests?
            $queryTrans = array('_id' => new MongoID($value));
            $transaction1 = $collection->findOne($queryTrans);          
            if($transaction1 && matchTransactions($transaction, $transaction1)) {
                return $transaction1;
            }
        }
    }

    function createPending($transaction) {
        global $collection;
        $transaction["type"] = u(PENDING)[T];
        $transaction["type_code"] = u(PENDING)[C];
        $transaction["date"] = new MongoDate();
        $collection->insert($transaction);
        $querySubId = array('_id' => new MongoID($transaction['subId0']));
        $collection->update($querySubId, 
            array('$push' => array("pending" => $transaction['_id']->{'$id'}))
        );
        return $transaction['_id']->{'$id'};
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
        if($pending) {
            $collection->update($querySubId1, array('$pull' => array("pending" => $pending['_id']->{'$id'})));
            $collection->remove(array('_id' => $pending['_id']));           
        }
        $transaction["type"] = u(TRANS)[T];
        $transaction["type_code"] = u(TRANS)[C];
        $transaction["date"] = new MongoDate();
        $collection->insert($transaction);
        $queryId0 = array('owner_id' => $transaction['id0']);   
        $collection->update($queryId0, array('$push' => array("transactions" => $transaction['_id']->{'$id'})));
        $queryId1 = array('owner_id' => $transaction['id1']);
        $collection->update($queryId1, array('$push' => array("transactions" => $transaction['_id']->{'$id'})));
        return $transaction['_id']->{'$id'};        
    }

    /**
     * Creates a new account document of the given type for an authorized owner. 
     */
    function addAccountDocument($type, $root_code) {
        global $collection;
        $account = findAccount(UID);
        if (!$account) {
            $account = array("owner_id" => UID, "type" => $type[T], "type_code" => $type[C], 
                "subaccounts" => array(), "groups" => array(), "transactions" => array());
            $collection->insert($account);
            updateTreeRoot(true, $root_code, $account['_id']->{'$id'});
            u(OUT)->success("Added account", $account['_id']->{'$id'}, NULL);
        } else {
            u(OUT)->failure("Account already exists");
        }
    }

    /**
     * Removes account record for given euid if it exists.
     */
    function removeAccountDocument($type, $root_code) {
        global $collection;
        $account = findAccount(UID);
        if ($account && $account["type_code"] == $type[C]) {
            updateTreeRoot(false, $root_code, $account['_id']->{'$id'});
            $collection->remove(array("owner_id" => UID));
            u(OUT)->success("Removed account", NULL, $account['_id']->{'$id'});
        } else {
            u(OUT)->failure("Could not find account");
        }   
    }

    function updateTreeRoot($add, $root_code, $document) {
        global $collection;
        $root_query = array("type_code" => $root_code);
        if($add) {  
            $collection->update($root_query, 
                array('$push' => array("accounts" => $document)));
        } else {
            $collection->update($root_query, 
                array('$pull' => array("accounts" => $document)));
        }
    }

    /**
     * Adds a new subaccount for the account with the given euid.
     * It creates a subaccount record and adds its _id to the account's
     * subaccount array.
     */
    function addSubaccount() {
        global $collection, $TYPE_SUBACCOUNT;
        $query = array("owner_id" => UID);
        if($collection->findOne($query)) {
            $subaccount = array("type" => u(SUBACCOUNT)[T], "type_code" => u(SUBACCOUNT)[C],
                "parent" => UID, "items" => array(), "pending" => array());
            if(isset($_GET["desc"])) $subaccount["desc"] = $_GET["desc"];
            $collection->insert($subaccount);
            $collection->update($query, 
                array('$push' => array("subaccounts" => $subaccount['_id']->{'$id'}))
            );
            u(OUT)->success("Added subaccount", $subaccount['_id']->{'$id'}, NULL);
        } else {
            u(OUT)->failure("Could not find account");
        }
    }

    /**
     * Removes the given subaccount record and its reference in the subaccounts
     * array of its parent account.
     */
    function removeSubaccount() {
        global $collection; 
        if (!findAccount(UID)) {
            u(OUT)->failure("Could not find account");
            return;
        } 
        $subId = $_GET["subId"];
        $subaccount = findSubaccount($subId);
        if (!$subaccount || $subaccount["parent"] != UID) {
            u(OUT)->failure("Could not find subaccount");
        } else {
            $queryId = array("owner_id" => UID);
            $querySubaccount = array('_id' => new MongoID($subId));
            $collection->update($queryId, array('$pull' => array("subaccounts" => $subId)));
            $collection->remove($querySubaccount);
            u(OUT)->success("Removed subaccount", NULL, $subId); //orphaned subaccount contents??
        }
    }

    // // function createGroup() {

    // // }

    // // function destroyGroup() {
        
    // // }

    // // function addGroupMember() {
        
    // // }

    // // function removeGroupMember() {
        
    // // }

    // // function changeGroupPermission() {

    // // }

    // function getTransactionHistory() {
    //  global $id, $output;
    //  $accountDoc = findAccount($id); 
    //  if($accountDoc == NULL) {
    //      $output->failure("Could not find account"); 
    //      return;     
    //  }
    //  $transactions = array("transactions" => $accountDoc["transactions"]);
    //  $output->success($transactions, NULL, NULL);    
    // }

    // function getTransactionDetail() { //TODO-----------------------------------------------------------------------------------------
    //  global $output, $collection;
    //  if($auth->isAdmin()) { //OR id0 or id1 is global id
    //      $transId = $_GET["transId"];
    //      $query= array('_id' => new MongoID($transId));
    //      $transDoc = $collection->findOne($query);
    //      if(NULL == $transDoc) {
    //          $output->failure("Transaction not found.");     
    //      } else {
    //          $output->success($transDoc, NULL, NULL);                    
    //      }
    //  } else {
    //          $output->failure("Init not authorized.");           
    //  }
    // }

    // function findTransactionInHistory() {
    //  global $id, $output, $collection;
    //  $accountDoc = findAccount($id);
    //  if(NULL == $accountDoc) {
    //      $output->failure("Could not find account");             
    //  }       
    // }

    // function getAccountInfo() {
    //  global $id, $output;
    //  $accountDoc = findAccount($id); 
    //  if($accountDoc == NULL) {
    //      $output->failure("Could not find account"); 
    //      return;     
    //  }
    //  $output->success($accountDoc, NULL, NULL);
    // }

    function validAcctAndSubacct($id, $subId, $id_name="", $subId_name="") {
        $accountDoc = findAccount($id); 
        if(!$accountDoc) {
            u(OUT)->failure("Could not find account " . $id_name);
        } elseif(!findSubaccount($subId)) {
            u(OUT)->failure("Could not find subaccount " . $subId_name);
        } elseif(!accountContainsSubaccount($accountDoc, $subId)) {
            u(OUT)->failure("Could not find subId for id " . $id_name);
        }
        else return TRUE;
    }

    // function getSubaccountInfo() {
    //  global $id, $output;
    //  if(!verifyAccountAndSubaccount($id, $_GET["subId"])) {
    //      return;
    //  }
    //  $sub = findSubaccount($_GET["subId"]);
    //  $output->success(findSubaccount($_GET["subId"]), NULL, NULL);
    // }

    function addItemToSubAccount() { //Check isAdmin?
        global $collection;
        $subId = $_GET["subId"];
        if(validAcctAndSubacct(UID, $subId)) {
            $item = $_GET["itemId"];
            $querySubId = array('_id' => new MongoID($subId));
            $collection->update($querySubId, array('$push' => array("items" => $item)));
            u(OUT)->success("Added item to subaccount", $item, NULL);   
        }
    }

    function removeItemFromSubAccount() {
        global $collection;
        if(!validAcctAndSubacct(UID, $_GET["subId"])) return;
        $subId = $_GET["subId"];
        $item = $_GET["itemId"];
        $querySubId = array('_id' => new MongoID($subId), 'items' => $item);
        if(!$collection->findOne($querySubId)) {
            u(OUT)->failure("No item itemId in subId");
            return;         
        }
        $collection->update($querySubId, array('$pull' => array("items" => $item)));
        u(OUT)->success("Removed item from subaccount", NULL, $item);       
    }

    // function getTreeRootInfo() {
    //  global $ROOT_TYPE, $TRANSACTION_ROOT, $collection, $output;
    //  $query = array($ROOT_TYPE => $TRANSACTION_ROOT);
    //  $root = $collection->findOne($query);
    //  if($root == NULL) {
    //      $output->failure("Tree root not found");
    //      return;         
    //  }
    //  $output->success($root, NULL, NULL);        
    // }

    // function getUserRootInfo() {
    //  global $ROOT_TYPE, $USER_ROOT, $collection, $output;
    //  $query = array($ROOT_TYPE => $USER_ROOT);
    //  $root = $collection->findOne($query);
    //  if($root == NULL) {
    //      $output->failure("User root not found");
    //      return;         
    //  }
    //  $output->success($root, NULL, NULL);            
    // }

    // function getGroupRootInfo() {
    //  global $ROOT_TYPE, $GROUP_ROOT, $collection, $output;
    //  $query = array($ROOT_TYPE => $GROUP_ROOT);
    //  $root = $collection->findOne($query);
    //  if($root == NULL) {
    //      $output->failure("Group root not found");
    //      return;         
    //  }
    //  $output->success($root, NULL, NULL);            
    // }

    // function getGlobalRootInfo() {
    //  global $ROOT_TYPE, $GLOBAL_ROOT, $collection, $output;
    //  $query = array($ROOT_TYPE => $GLOBAL_ROOT);
    //  $root = $collection->findOne($query);
    //  if($root == NULL) {
    //      $output->failure("Global root not found");
    //      return;         
    //  }
    //  $output->success($root, NULL, NULL);
    // }

    function findAccount($owner_id, $type_code=FALSE) {
        global $collection;
        if($type_code) {
            return $collection->findOne(array('owner_id' => $owner_id, 'type_code' => $type_code));
        } else return $collection->findOne(array('owner_id' => $owner_id));
    }

    function accountContainsSubaccount($accountDoc, $subId) {
        foreach ($accountDoc['subaccounts'] as $value) {
            if($value == $subId) return TRUE;
        }
        return FALSE;
    }

    // function accountContainsTransaction() {} //TODO???-------------------------------------------------------------------

    function findSubaccount($subId) {
        global $collection;
        try {
            $querySubId = array('_id' => new MongoID($subId));
            $subaccount = $collection->findOne($querySubId);
            if($subaccount && $subaccount['type_code'] == u(SUBACCOUNT)[C]) {
                return $subaccount;
            }
        } catch (MongoException $m) {}
    } 

    function setDbName() {
        if(isset($_GET["testing"]) && $_GET["testing"] === "true") {
            return "LuxTest";
            echo 'testing';
        } else return "Lux";
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
        global $collection;
        $root = $collection->findOne(array("type_code" => u(TREE_ROOT)[C]));
        $user = $collection->findOne(array("type_code" => u(USER_ROOT)[C]));
        $global = $collection->findOne(array("type_code" => u(GLOBAL_ROOT)[C]));
        $group = $collection->findOne(array("type_code" => u(GROUP_ROOT)[C]));
        return $root & $user && $global && $group;
    }

    function u($arr) {
        return unserialize($arr);
    }

?>