<?php 
    ini_set('display_errors',1);
    ini_set('display_startup_errors',1);
    error_reporting(-1);
    
    function makeRequest($method, $query) {
        $url = "http://localhost:8888/ts/transactionServer.php?method=" . $method . "&testing=true" . $query;
        return json_decode(file_get_contents($url));
    }

    function clear() {
        $m = new MongoClient();
        $db = $m->selectDB("LuxTest");
        $coll = $db->selectCollection("transaction");
        $coll->remove(array());
        $m->close();
    }
    
    function testInitTreeStruct() {
        echo "Test initTreeStruct<br>";
        clear();
        $result =  makeRequest("initTreeStruct", "&acc_token=123");
        assert($result->{"result"} == "failure"); //Not admin, not authorized
        echo json_encode($result) . "<br>";
        $result =  makeRequest("initTreeStruct", "&acc_token=123&admin"); 
        assert($result->{"result"} == "success"); //Authorized and cleared
        echo json_encode($result) . "<br>";
        $result =  makeRequest("initTreeStruct", "&acc_token=123&admin"); 
        assert($result->{"result"} == "failure"); //Authorized but not cleared
        echo json_encode($result) . "<br>";
        $result =  makeRequest("initTreeStruct", "&acc_token=123&admin&force=true"); 
        assert($result->{"result"} == "success"); //Authorized and forced
        echo json_encode($result) . "<br><br>";
    }

    function testAddandRemoveUserAccount() {
        echo "Test addUserAccount and removeUserAccount<br>";
        makeRequest("initTreeStruct", "&acc_token=1&admin&force=true");
        $result =  makeRequest("addUserAccount", "&acc_token=123"); 
        assert($result->{"result"} == "success"); //New account
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addUserAccount", "&acc_token=123"); 
        assert($result->{"result"} == "failure"); //Already exists
        echo json_encode($result) . "<br>";
        $result =  makeRequest("removeUserAccount", "&acc_token=123"); 
        assert($result->{"result"} == "success"); //Removed
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addUserAccount", "&acc_token=123"); 
        assert($result->{"result"} == "success"); //New account
        echo json_encode($result) . "<br>";
        $result =  makeRequest("removeUserAccount", "&acc_token=124"); 
        assert($result->{"result"} == "failure"); //Doesn't exist
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addUserAccount", "&acc_token=124"); 
        assert($result->{"result"} == "success"); //New account
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addUserAccount", "&acc_token=124"); 
        assert($result->{"result"} == "failure"); //Already exists
        echo json_encode($result) . "<br><br>";
    }

    function testAddandRemoveGlobalAccount() {
        echo "Test addUserAccount and removeUserAccount<br>";
        makeRequest("initTreeStruct", "&acc_token=1&admin&force=true");
        $result =  makeRequest("addGlobalAccount", "&acc_token=123"); 
        assert($result->{"result"} == "failure"); //Not admin
        echo json_encode($result) . "<br>";
        $result =  makeRequest("removeGlobalAccount", "&acc_token=123&admin"); 
        assert($result->{"result"} == "failure"); //Doesn't exist
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addGlobalAccount", "&acc_token=123&admin"); 
        assert($result->{"result"} == "success"); //Authorized
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addGlobalAccount", "&acc_token=123&admin"); 
        assert($result->{"result"} == "failure"); //Already exists - should admin id have >1 global account?
        echo json_encode($result) . "<br>";
        $result =  makeRequest("removeGlobalAccount", "&acc_token=123&admin"); 
        assert($result->{"result"} == "success"); //Removed
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addGlobalAccount", "&acc_token=123&admin"); 
        assert($result->{"result"} == "success"); //Authorized
        echo json_encode($result) . "<br><br>";
    }

    function testAddandRemoveSubaccount() {
        echo "Test addSubaccount and removeSubaccount<br>";
        makeRequest("initTreeStruct", "&acc_token=1&admin&force=true");
        $result =  makeRequest("addSubaccount", "&acc_token=123"); 
        assert($result->{"result"} == "failure"); //No account
        echo json_encode($result) . "<br>";
        $result =  makeRequest("removeSubaccount", "&acc_token=123&subId=5"); 
        //assert($result->{"result"} == "failure"); //No subaccount
        echo json_encode($result) . "<br>";
        $result = makeRequest("addUserAccount", "&acc_token=123");
        echo json_encode($result) . "<br>";
        $result = makeRequest("addUserAccount", "&acc_token=124");
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addSubaccount", "&acc_token=124"); 
        assert($result->{"result"} == "success"); //Added to user id 124
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addSubaccount", "&acc_token=123"); 
        assert($result->{"result"} == "success"); //Added to user id 123
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addSubaccount", "&acc_token=123"); 
        assert($result->{"result"} == "success"); //Added second to user id 123
        echo json_encode($result) . "<br>";
        $subaccount = $result->{"added"};
        $query = "&acc_token=124&subId=" . $subaccount;
        $result =  makeRequest("removeSubaccount", $query); 
        assert($result->{"result"} == "failure"); //No subaccount - wrong account
        echo json_encode($result) . "<br>";
        $query = "&acc_token=123&subId=" . $subaccount;
        $result =  makeRequest("removeSubaccount", $query); 
        assert($result->{"result"} == "success"); //Removed second from user id 123
        echo json_encode($result) . "<br>";
        $result =  makeRequest("removeSubaccount", $query); 
        assert($result->{"result"} == "failure"); //Already removed
        echo json_encode($result) . "<br><br>";
    }

    function testTransactionIncompleteDetails() {
        echo "Test makeTransaction incomplete details<br>";
        makeRequest("initTreeStruct", "&acc_token=1&admin&force=true");
        $query = array( 
            "acc_token" => "111",
            "id1" => "222",
            "subId0" => "333",
            "subId1" => "444",
            "itemsId0" => array(),
            "itemsId1" => ["777", "888"]);
        $query_string = "&" . http_build_query($query);
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "failure"); //No id0 account
        echo json_encode($result) . "<br>";
        makeRequest("addUserAccount", "&acc_token=111");
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "failure"); //No subId0
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addSubaccount", "&acc_token=111"); 
        $query["subId0"] = $result->{"added"};
        $query_string = "&" . http_build_query($query);
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "failure"); //No id1 account
        echo json_encode($result) . "<br>";
        makeRequest("addUserAccount", "&acc_token=222");
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "failure"); //No subId1
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addSubaccount", "&acc_token=111"); 
        $query["subId1"] = $result->{"added"};
        $query_string = "&" . http_build_query($query);
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "failure"); //SubId1 in wrong account
        echo json_encode($result) . "<br><br>";
    }

    function testAddItemToSubaccount() {
        echo "Test addItemToSubaccount<br>";
        makeRequest("initTreeStruct", "&acc_token=1&admin&force=true");
        $result = makeRequest("addItemToSubaccount", "&acc_token=111&subId=2&itemId=3");
        assert($result->{"result"} == "failure"); //No id account
        echo json_encode($result) . "<br>";
        makeRequest("addUserAccount", "&acc_token=111");
        $result = makeRequest("addItemToSubaccount", "&acc_token=111&subId=2&itemId=3");
        assert($result->{"result"} == "failure"); //No subaccount
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addSubaccount", "&acc_token=111"); 
        $subId = $result->{"added"};
        $result = makeRequest("addItemToSubaccount", "&acc_token=111&subId=" . $subId);
        assert($result->{"result"} == "failure"); //No item
        echo json_encode($result) . "<br>";
        makeRequest("addUserAccount", "&acc_token=222");
        $result = makeRequest("addItemToSubaccount", "&acc_token=222&subId=" . $subId);
        assert($result->{"result"} == "failure"); //No subaccount for account
        echo json_encode($result) . "<br>";
        $result = makeRequest("addItemToSubaccount", "&acc_token=111&subId="
            . $subId . "&itemId=555");
        assert($result->{"result"} == "success"); //Added to user
        echo json_encode($result) . "<br>";
        $result = makeRequest("addItemToSubaccount", "&acc_token=111&subId="
            . $subId . "&itemId=666");
        assert($result->{"result"} == "success");
        echo json_encode($result) . "<br>";
        makeRequest("addGlobalAccount", "&acc_token=333&admin"); 
        $result =  makeRequest("addSubaccount", "&acc_token=333"); 
        $subId = $result->{"added"};
        $result = makeRequest("addItemToSubaccount", "&acc_token=333&subId="
            . $subId . "&itemId=777");
        assert($result->{"result"} == "success"); //Added to global
        echo json_encode($result) . "<br><br>";
    }

    function testMakeTransactionWithGlobal() {
        echo "Test transaction with global<br>";
        //User to Global
        makeRequest("initTreeStruct", "&acc_token=1&admin&force=true");
        makeRequest("addUserAccount", "&acc_token=111");
        makeRequest("addGlobalAccount", "&acc_token=222&admin"); 
        $result = makeRequest("addSubaccount", "&acc_token=111"); 
        $subId0 = $result->{"added"};
        $result =  makeRequest("addSubaccount", "&acc_token=222");
        $subId1 = $result->{"added"};
        $query = array( 
            "acc_token" => "111",
            "id1" => "222",
            "subId0" => $subId0,
            "subId1" => $subId1,
            "itemsId1" => ["101"]);
        $query_string = "&" . http_build_query($query);
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "failure"); //Missing item 101
        echo json_encode($result) . "<br>";
        $result = makeRequest("addItemToSubaccount", "&acc_token=222&subId="
            . $subId1 . "&itemId=101");
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "success"); //Added to global
        echo json_encode($result) . "<br>";
        $query = array( 
            "acc_token" => "111",
            "id1" => "222",
            "subId0" => $subId0,
            "subId1" => $subId1,
            "itemsId0" => ["101", "102"],
            "itemsId1" => ["103"]);
        $query_string = "&" . http_build_query($query);
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "failure"); //Missing 102, 103
        echo json_encode($result) . "<br>";
        $result = makeRequest("addItemToSubaccount", "&acc_token=222&subId="
            . $subId1 . "&itemId=103");
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "failure"); //Missing 102
        echo json_encode($result) . "<br>";
        $result = makeRequest("addItemToSubaccount", "&acc_token=111&subId="
            . $subId0 . "&itemId=102");
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "success");
        echo json_encode($result) . "<br>";

        //Global to Global
        makeRequest("addGlobalAccount", "&acc_token=333&admin"); 
        $result = makeRequest("addSubaccount", "&acc_token=333"); 
        $subId2 = $result->{"added"};
        $query = array( 
            "acc_token" => "222",
            "id1" => "333",
            "subId0" => $subId1,
            "subId1" => $subId2,
            "itemsId0" => ["101"]);
        $query_string = "&" . http_build_query($query);
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "success");
        echo json_encode($result) . "<br><br>";
    }

    function testMakePendingTransaction() {
        echo "Test make pending transaction<br>";
        makeRequest("initTreeStruct", "&acc_token=1&admin&force=true");
        makeRequest("addUserAccount", "&acc_token=111");
        makeRequest("addUserAccount", "&acc_token=222"); 
        $result = makeRequest("addSubaccount", "&acc_token=111"); 
        $subId0 = $result->{"added"};
        $result =  makeRequest("addSubaccount", "&acc_token=222");
        $subId1 = $result->{"added"};
        $result = makeRequest("addItemToSubaccount", "&acc_token=222&subId="
            . $subId1 . "&itemId=101");
        $query = array( 
            "acc_token" => "111",
            "id1" => "222",
            "subId0" => $subId0,
            "subId1" => $subId1,
            "itemsId1" => ["101"]);
        $query_string = "&" . http_build_query($query);
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "success");
        echo json_encode($result) . "<br>";
        $query = array( 
            "acc_token" => "222",
            "id1" => "111",
            "subId0" => $subId1,
            "subId1" => $subId0,
            "itemsId0" => ["101", "102"]);
        $query_string = "&" . http_build_query($query);
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "failure"); //No 102
        echo json_encode($result) . "<br>";
        $result = makeRequest("addItemToSubaccount", "&acc_token=222&subId="
            . $subId1 . "&itemId=102");
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "success");
        echo json_encode($result) . "<br>";
        $query = array( 
            "acc_token" => "111",
            "id1" => "222",
            "subId0" => $subId0,
            "subId1" => $subId1,
            "itemsId1" => ["101", "102"]);
        $query_string = "&" . http_build_query($query);
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "success");
        echo json_encode($result) . "<br><br>";
    }

    function testRemoveItemFromSubaccount() {
        echo "Test remove item from subaccount<br>";
        makeRequest("initTreeStruct", "&acc_token=1&admin&force=true");
        $result = makeRequest("removeItemFromSubaccount", "&acc_token=000&subId=2&itemId=3");
        assert($result->{"result"} == "failure"); //No id account
        echo json_encode($result) . "<br>";
        makeRequest("addUserAccount", "&acc_token=000");
        makeRequest("addGlobalAccount", "&acc_token=111&admin"); 
        $result = makeRequest("removeItemFromSubaccount", "&acc_token=000&subId=2&itemId=3");
        assert($result->{"result"} == "failure"); //No subaccount
        echo json_encode($result) . "<br>";
        $result =  makeRequest("addSubaccount", "&acc_token=000"); 
        $subId0 = $result->{"added"};
        $result =  makeRequest("addSubaccount", "&acc_token=111"); 
        $subId1 = $result->{"added"};
        $result = makeRequest("removeItemFromSubaccount", "&acc_token=000&subId=" . $subId0);
        assert($result->{"result"} == "failure"); //No item
        echo json_encode($result) . "<br>";
        $result = makeRequest("addItemToSubaccount", "&acc_token=000&subId="
            . $subId0 . "&itemId=555");
        assert($result->{"result"} == "success"); //Added to user
        echo json_encode($result) . "<br>";
        $result = makeRequest("addItemToSubaccount", "&acc_token=000&subId="
            . $subId0 . "&itemId=666");
        assert($result->{"result"} == "success"); //Added to user
        echo json_encode($result) . "<br>";
        $result = makeRequest("removeItemFromSubaccount", "&acc_token=111&subId="
            . $subId1 . "&itemId=555");
        assert($result->{"result"} == "failure"); //Item not in subaccount
        echo json_encode($result) . "<br>";
        $result = makeRequest("removeItemFromSubaccount", "&acc_token=000&subId="
            . $subId0 . "&itemId=555");
        assert($result->{"result"} == "success"); //Item removed
        echo json_encode($result) . "<br><br>";
    }

    function testMakeTransactionWithUser() {
        echo "Test transaction with user<br>";
        makeRequest("initTreeStruct", "&acc_token=1&admin&force=true");
        makeRequest("addUserAccount", "&acc_token=000");
        makeRequest("addUserAccount", "&acc_token=111"); 
        $result =  makeRequest("addSubaccount", "&acc_token=000"); 
        $subId0 = $result->{"added"};
        $result =  makeRequest("addSubaccount", "&acc_token=111"); 
        $subId1 = $result->{"added"};
        $result = makeRequest("addItemToSubaccount", "&acc_token=000&subId="
            . $subId0 . "&itemId=0.0");
        $result = makeRequest("addItemToSubaccount", "&acc_token=000&subId="
            . $subId0 . "&itemId=0.1");
        $result = makeRequest("addItemToSubaccount", "&acc_token=111&subId="
            . $subId1 . "&itemId=0.2");
        $result = makeRequest("addItemToSubaccount", "&acc_token=000&subId="
            . $subId0 . "&itemId=1.0");
        $result = makeRequest("addItemToSubaccount", "&acc_token=000&subId="
            . $subId0 . "&itemId=1.1");
        $result = makeRequest("addItemToSubaccount", "&acc_token=111&subId="
            . $subId1 . "&itemId=1.2");
        $query = array( 
            "acc_token" => "000",
            "id1" => "111",
            "subId0" => $subId0,
            "subId1" => $subId1,
            "itemsId0" => ["1.0", "1.1"],
            "itemsId1" => ["0.2"]);
        $query_string = "&" . http_build_query($query);
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "success");
        echo json_encode($result) . "<br>";
        $query = array( 
            "acc_token" => "111",
            "id1" => "000",
            "subId0" => $subId1,
            "subId1" => $subId0,
            "itemsId0" => ["0.2"],
            "itemsId1" => ["1.0", "1.1"]);
        $query_string = "&" . http_build_query($query);
        $result = makeRequest("transaction", $query_string);
        assert($result->{"result"} == "success");
        echo json_encode($result) . "<br><br>";
    }

    testInitTreeStruct();
    testAddandRemoveUserAccount();
    testAddandRemoveGlobalAccount();
    testAddandRemoveSubaccount();
    testTransactionIncompleteDetails();
    testAddItemToSubaccount();
    testMakeTransactionWithGlobal();
    testMakePendingTransaction();
    testRemoveItemFromSubaccount();
    testMakeTransactionWithUser();

?>  
