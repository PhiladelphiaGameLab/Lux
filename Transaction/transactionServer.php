<?php

	include 'Transaction.php';

	if(!isset($_GET["method"])) {
		echo "No method specified.";
		return;
	}
	switch ($_GET["method"]) {

		case "initTreeStruct":
			if(isset($_GET["force"]) && strcmp($_GET["force"], "true") == 0) {
				initTreeStruct(true);
			} else {
				initTreeStruct(false);
			}
			break;
		case "transaction":
			transaction();
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

	function initTreeStruct($force) {
		if($force) {
			echo "force";
		} else {
			echo "don't force";
		}
	}

	function transaction() {
		$trans = Transaction::constructFromQuery();
		$trans->printDetails();
	}

	function addAccount() {

	}

	function removeAccount() {
		
	}

	function addSubaccount() {
		
	}

	function removeSubaccount() {
		
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