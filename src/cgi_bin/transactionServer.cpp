// check that references are in id's subaccount
// 1) Unmatched request (id0 == id1 || id1 is global type)
//     -check id1 item references
//     -loop and swap references
//     -reply successful   
// 2) Matched request (between two players)
//     -check until matched or failed
//     -if matched
//         -loop and swap references
//         -reply successful to both
// else reply fail

#include "../classes/static/Authenticate.h"
#include "../classes/instanceable/CGI.h"
#include "../mongodb/mongoWrapper.h"
#include "mongo/bson/bson.h"
#include <string>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <stdlib.h>

using namespace std;
using namespace mongo;

// Transaction info
// 
// items id should be in the form {items : [xxx,xxx,xxx]}
struct TransactionInfo {
    string id0; 
    string id1;
    string subId0;
    string subId1;
    string itemsId0;
    string itemsId1;
    TransactionInfo(string i0, string i1,
                    string si0, string si1,
                    string iti0, string iti1):
        id0(i0), id1(i1), subId0(si0), subId1(si1),
        itemsId0(iti0), itemsId1(iti1) {};
    TransactionInfo(){};
};

// global varibales

// get parameters from the HTTP Request
CGI *environment;

// connection to mongo db
MongoWrapper *conn;

// the namespace of collection
const string db_ns("db.transaction"); 

const string OBJ_ID("_id");

// Account type
const string ACC_TYPE_FILED("type");
enum AccType{
    TYPE_ACCOUNT = 1,
    TYPE_SUB_ACCOUNT = 2,
    TYPE_GLOBAL_ACCOUNT = 4,
    TYPE_GROUP_ACCOUNT = 8
};

enum AccRelation {
    PARENT,
    MEMBER,
    ADMIN,    
};

// Tree structure
//                  Root
//   |---------------|--------------|
// User Root    Group Root     Global Root
//  |               |              |
// Acc ...       Acc ...         Acc...
// |               |              |
// Sub acc...    Sub acc ...    Sub Acc..

// Root structure
// {
//   "_id" : ...,
//   "root_type" : "TRANSACTION_ROOT",
//   "User_Root" : ObjectId(),
//   "Group_Root" : ObjectId(),
//   "Global_Root" : ObjectId()
// }

// User/Group/Global Root Structure
// {
//   "_id" : ...,
//   "root_type" : "USER/GROUP/GLOBAL_ROOT",
//   "accounts" : [ ] // array of accounts ObjectIds
// }

const string ROOT_TYPE_FIELD("root_type");
const string TRANSACTION_ROOT("Transaction_Root");
const string USER_ROOT("User_Root");
const string GROUP_ROOT("Group_Root");
const string GLOBAL_ROOT("Global_Root");
const string CHILD_ARRAY_FILED("accounts");

// Account structure
// e.g.
// {
//  "_id" : "xxx", (created by mongo)
//  "name" : username,
//  "type" : ACCOUNT, or GLOBAL_ACCOUNT,
//  "group" : [],  // global account doesn't have group
//  "sub_acc" : [],
//  "trans_history" : []
//  }
// the field name for account id
const string ACC_ID_FIELD("_id"); 
// filed name for username in account document
const string USER_NAME_FIELD("name");
// filed name for group account
const string GROUP_ACC_ARRAY_FIELD("group");
// sub account array in account
const string SUB_ACC_ARRAY_FILED("sub_acc_array");
// transaction history array in account
const string TRANSACTION_HISTORY("completed_array");

// Sub account structure
// e.g.
// {
//  "_id" : "xxx", (created by mongo)
//  "type" : SUB_ACCOUNT,
//  "parent" : _id (account id),
//  "items" : [],
//  "pending" : []
// }
// field name for subaccount, default is equal to account id
const string SUB_ACC_ID_FIELD(ACC_ID_FIELD);
const string SUB_PARENT_FIELD("parent");
// items array in sub account
const string ITEM_ARRAY_FIELD("items");
// Field name of pending array
const string PENDING_ARRAY("pending");

// flag indicates whether to log self transaction
const bool LOG_SELF_TRANSACTION = false;

// Group account structure
// e.g.
// {
//  "_id" : "xxx", (created by mongo)
//  "type" : AccType.GROUP_ACCOUNT,
//  "admin" : [], (userids)
//  "member" : [] (userids)
// }
const string GROUP_ID_FIELD("_id");
const string ADMIN_ARRAY_FIELD("admin");
const string MEMBER_ARRAY_FIELD("member");

// Transaction log object fields:
// e.g.
// {
//   player0 : id0,
//   player1 : id1,
//   subacc0 : subid0,
//   subacc1 : subid1,
//   itemslist0 : items0,
//   itemslist1 : items1,
//   cerateTime : time
// }
const string PLAYER0("p0");
const string PLAYER1("p1");
const string SUBACC0("s0");
const string SUBACC1("s1");
const string ITEMLIST0("items0");
const string ITEMLIST1("items1");
const string CREATE_TIME("time");

const BSONArray EMPTY_JSON_ARRAY = BSON_ARRAY("array");


// Time to wait for matching pending transaction
const int TOTAL_TIME = 10; // 10 seconds
int time_cnt = 0;

int handleRequest();
bool findSubAcc(const TransactionInfo &info, BSONObj &subAcc0, BSONObj &subAcc1);
bool findSubAccInAcc(const BSONObj &acc, const string &index, BSONObj &subAcc);
bool checkItemIDs(const BSONObj &subAcc, const BSONObj &items);
bool checkMatch(const TransactionInfo &info, BSONObj &subAcc0, BSONObj &subAcc1);
void addPendingTransaction(const string &accID, const TransactionInfo &info);
void removePendingTransaction(const string &accID, const TransactionInfo &info);
bool hasPendingTransaction(const string &accId, const TransactionInfo &info);
void makeInfoFromTransaction(BSONElement &pending, TransactionInfo &info);
bool matchRequest(const TransactionInfo &sending, 
		  const TransactionInfo &receiving);
void executeTransaction(const TransactionInfo &info);

void updateItemArray(const string method, const string &accID, 
		     const string &itemIDs);
string findTransaction(const TransactionInfo &info, int senderId);
void removeTransaction(const string id);
void logTransaction(const TransactionInfo &info);
//for both users if player to player
void addCompletedTransaction(const TransactionInfo &info, int flag); 
void reverseInfo(TransactionInfo &info);

bool isGlobalAccount(const string &id);
void getAsset();
void getAccountInfo(const string &id);
void getSubAccountInfo(const string &id, const string &subId);
void getTransactionHistory(const string &id);

void ensureAccountType(const BSONObj &acc, int accType);
void ensureRelationship(const BSONObj &acc0, const BSONObj &acc1, int relation);
void ensureGroupRelations(const string &gid, 
			  const string &id0, const string &id1);
bool findStringInArray(vector<BSONElement> &array, const string &str);
void addAccount(const string &username);
void removeAccount(const string &id);
void addGlobalAccount(const string &name);
void removeGlobalAccount(const string &id);
void addSubAccount(const string &id);
void removeSubAccount(const string &id, const string &subId);
void clearSubAccount(const string &subId);
void createGroup(const string &id);
void destroyGroup(const string &gid);
void addGroupMember(const string &gid, const string &id0, const string &id1);
void removeGroupMember(const string &gid, const string &id0, const string &id1);
void changeGroupPermission(const string &gid,
			   const string &id0, const string &id1,
			   int permission);
void addItemToSubAccount(const string &subId, const string &itemId);
void removeItemFromSubAccount(const string &subId, const string &itemId);
void error(const string &msg);
void sendJson(const string &jsonString);

void initTreeStruct(bool force);
void ensureInitTree();
void getTreeRootInfo();
void getUserRootInfo();
void getGroupRootInfo();
void getGlobalRootInfo();


// Helper function to get rid of "" of a filed value of string
inline string getFieldValue(const BSONObj &obj, const string &field) {
    // get rid of "" surrounding a string
    // e.g. ""hello"" -> "hello"
    string str = obj[field].toString(false);
    if (str[0] == '\"') {
	str = str.substr(1, str.size() - 2);
    }
    return str;
}

inline string getFieldValue(const BSONElement &elem, const string &field = "") {
    // get rid of "" surrounding a string
    // e.g. ""hello"" -> "hello"
    string str(elem.toString(false));
    if (field.size() > 0) {
	str = elem[field].toString(false);
    }
    if (str[0] == '\"') {
	str = str.substr(1, str.size() - 2);
    }
    return str;
}

inline bool timedOut() {
    return (time_cnt > TOTAL_TIME);
}

inline string getCurrentTime() {
    time_t rawtime;
    time(&rawtime);
    // ctime convert time_t value to string
    // Www Mmm dd hh:mm:ss yyyy
    // e.g. Wed Jun  4 15:44:25 2014
    return time_t_to_String(rawtime);
}

int main(int argc, char **argv) {
    /*
    string EUID = environment->get("EUID");
    string accessToken = environment->get("accessToken");

    if (!Authenticate::authenticateAccessToken(accessToken, EUID)) {
    	return 1;
    }
    */

    environment = new CGI();        
    conn = new MongoWrapper("localhost");

    handleRequest();

    return 0;
}

int handleRequest() {

    string method = environment->get("method");
    
    if (method.compare("initTreeStruct") != 0) {
	ensureInitTree();
    }

    if(method.compare("transaction") == 0) {
        string id0 = environment->get("id0"); //verify id0 == EUID or use EUID directly?
        string id1 = environment->get("id1");
        string subId0 = environment->get("subId0");
	string subId1 = environment->get("subId1");
        string itemsId0 = environment->get("itemsId0");
	string itemsId1 = environment->get("itemsId1");
	
	// Normlize itemsId
	itemsId0 = fromjson(itemsId0).toString();
	itemsId1 = fromjson(itemsId1).toString();

	TransactionInfo info(id0, id1,
			     subId0, subId1,
			     itemsId0, itemsId1);	
	BSONObj subAcc0, subAcc1;
	
	findSubAcc(info, subAcc0, subAcc1);

        if (!checkItemIDs(subAcc0, fromjson(info.itemsId0))) {
            //failed error
	    error("Items don't match.");
        }
	if (!checkItemIDs(subAcc1, fromjson(info.itemsId1))) {
	    error("Items don't match.");
	}
	
	// Self transaction
	if (info.id0.compare(info.id1) == 0) {
	    executeTransaction(info);
	    string result = "{\"message\":\"Transaction successfully finished.\"}";
	    sendJson(result);		    
	    return 0;	    
	}
	
	// Player to global
	if (isGlobalAccount(info.id1)) {
	    executeTransaction(info);
	    string result = "{\"message\":\"Global Transaction successfully finished.\"}";
	    sendJson(result);
	    return 0;	    
	}	
	
	// Player to player
	if (subId0.compare(subId1) < 0) {
	    // Always let the bigger one do check match stuff
	    string accID = subId0;
	    addPendingTransaction(accID, info);
	    while (!timedOut()) {
		// Sleep and wait for the other player
		if (!hasPendingTransaction(accID, info)) {
		    break;
		}
		this_thread::sleep_for(chrono::seconds(1));
	    }
	    // Remove pending transaction when time out
	    // If this pending transaction is already been processed
	    // this function will do nothing.	    	    
	    if (hasPendingTransaction(accID, info)) {
		removePendingTransaction(accID, info);
		error("Transaction failed.");
	    }
	    else {
		string result = "{\"message\":\"Transaction successfully finished.\"}";
		sendJson(result);
	    }
	    return 0;
	}
	
	// The bigger one will check matched pending transaction 
	// several times untill time out
	while (!timedOut()) {
	    // Get the lastest subAcc1
	    subAcc1 = conn->findOne(db_ns, QUERY(SUB_ACC_ID_FIELD << OID(subId1)));
	    
	    if (checkMatch(info, subAcc0, subAcc1)) {
		executeTransaction(info);
		string result = "{\"message\":\"Transaction successfully finished.\"}";
		sendJson(result);		    
		return 0;
	    }

	    time_cnt++;
	    this_thread::sleep_for(chrono::seconds(1));
	}
	// Time out
	// Send back error message
	error("Transaction failed.");
	return 0;
    }
    else if (method.compare("addAccount") == 0) {
	//addAccount
	string username = environment->get("username");
	addAccount(username);
    } 
    else if (method.compare("removeAccount") == 0) {
	string id = environment->get("id0");
	removeAccount(id);
    } 
    else if (method.compare("addSubAccount") == 0) {
        //addSubaccount
	string id = environment->get("id0");
	addSubAccount(id);
    }
    else if (method.compare("removeSubAccount") == 0) {
	string id = environment->get("id0");
        string subId = environment->get("subId0");
	removeSubAccount(id, subId);
    }
    else if (method.compare("createGroup") == 0) {
	string id = environment->get("id0");
	createGroup(id);
    }
    else if (method.compare("destroyGroup") == 0) {
	string gid = environment->get("groupId");
	destroyGroup(gid);
    }
    else if (method.compare("addGroupMember") == 0) {
	string id0 = environment->get("id0");
	string id1 = environment->get("id1");
	string gid = environment->get("groupId");
	addGroupMember(gid, id0, id1);
    }
    else if (method.compare("removeGroupMember") == 0) {
	string id0 = environment->get("id0");
	string id1 = environment->get("id1");
	string gid = environment->get("groupId");
	removeGroupMember(gid, id0, id1);
    }
    else if (method.compare("changeGroupPermission") == 0) {
	string id0 = environment->get("id0");
	string id1 = environment->get("id1");
	string gid = environment->get("groupId");
	string permission = environment->get("permission");
	changeGroupPermission(gid, id0, id1, atoi(permission.c_str()));
    }
    else if (method.compare("getTransactionHistory") == 0) {
	string id = environment->get("id0");
	getTransactionHistory(id);
    }
    else if (method.compare("getAccountInfo") == 0) {
	string id = environment->get("id0");
	getAccountInfo(id);
    }
    else if (method.compare("getSubAccountInfo") == 0) {
	string id = environment->get("id0");
	string subId = environment->get("subId0");
	getSubAccountInfo(id, subId);
    }
    else if (method.compare("addItemToSubAccount") == 0) {
	string subId = environment->get("subId0");
	string itemId = environment->get("itemId");
	addItemToSubAccount(subId, itemId);	
    }
    else if (method.compare("removeItemFromSubAccount") == 0) {
	string subId = environment->get("subId0");
	string itemId = environment->get("itemId");
	removeItemFromSubAccount(subId, itemId);
    }
    else if (method.compare("addGlobalAccount") == 0) {
	//addAccount
	string name = environment->get("name");
	addGlobalAccount(name);
    } 
    else if (method.compare("removeGlobalAccount") == 0) {
	string id = environment->get("id0");
	removeGlobalAccount(id);
    } 
    else if (method.compare("initTreeStruct") == 0) {
	string force = environment->get("force");
	initTreeStruct(force == "true");
    }
    else if (method.compare("getTreeRootInfo") == 0) {
	getTreeRootInfo();
    }
    else if (method.compare("getUserRootInfo") == 0) {
	getUserRootInfo();
    }
    else if (method.compare("getGroupRootInfo") == 0) {
	getGroupRootInfo();
    }
    else if (method.compare("getGlobalRootInfo") == 0) {
	getGlobalRootInfo();
    }
    else if (method.compare("get") == 0) {
        //getAsset
	
    }
    else {
	error("Requested method doesn't exist.");
    }
    return 0;
}

// Find two subaccounts in the two accounts
bool findSubAcc(const TransactionInfo &info, 
		BSONObj &subAcc0, BSONObj &subAcc1) {
    BSONObj acc = conn->findOne(db_ns, QUERY(ACC_ID_FIELD << OID(info.id0)));

    if (findSubAccInAcc(acc, info.subId0, subAcc0) == false) {
	return false;
    }
    
    if (info.id0.compare(info.id1) == 0) {
	// Two accounts are the same
	if (info.subId0.compare(info.subId1) == 0) {
	    // If two sub accounts are the same, simply copy it
	    subAcc1 = subAcc0;
	    return true;
	}
	if (findSubAccInAcc(acc, info.subId1, subAcc1) == false) {
	    return false;		
	}
	else {
	    return true;
	}
    }
    
    // Two accounts are different
    acc = conn->findOne(db_ns, QUERY(ACC_ID_FIELD << OID(info.id1)));

    if (findSubAccInAcc(acc, info.subId1, subAcc1) == false) {
	return false;
    }
    
    return true;
}

// Find a subaccount in account
// Here I made the assumption that in an account object
// subaccounts' ids are stored in an array.
// e.g. Account: {"_id":xxxx, "sub_acc_array":[xxx,xxx,xxx,xxx]}.
bool findSubAccInAcc(const BSONObj &acc, const string &subId, BSONObj &subAcc) {
    ensureAccountType(acc, TYPE_ACCOUNT | TYPE_GLOBAL_ACCOUNT);
    
    subAcc = conn->findOne(db_ns, QUERY(SUB_ACC_ID_FIELD << OID(subId)));

    ensureAccountType(subAcc, TYPE_SUB_ACCOUNT);
    
    ensureRelationship(acc, subAcc, PARENT);

    return true;
}

// Check if all items are in a subaccount.
// Return flase if a least one is missing.
//
// Assuming item ids in sub account are stored in an array
// e.g. {sub_acc_id : xxx, items : [xxx,xxx,xxx]}
bool checkItemIDs(const BSONObj &subAcc, const BSONObj &items) {
    vector<BSONElement> itemArr0 = items.firstElement().Array();
    vector<BSONElement> itemArr1 = subAcc[ITEM_ARRAY_FIELD].Array();

    for (vector<BSONElement>::iterator it0 = itemArr0.begin() + 1; 
	 it0 != itemArr0.end(); 
	 ++it0){
	string value0(getFieldValue(*it0));
	bool find = false;
	for (vector<BSONElement>::iterator it1 = itemArr1.begin() + 1; 
	     it1 != itemArr1.end(); 
	     ++it1){

	    string value1(getFieldValue(*it1));
	    if (value0.compare(value1) == 0) {
		find = true;
		break;
	    }
	}
	if (!find) {
	    return false;
	}
    }
    return true;
}

// Check if the transaction between two accounts matches
bool checkMatch(const TransactionInfo &info, BSONObj &subAcc0, BSONObj &subAcc1) {    
    vector<BSONElement> pendingArr = subAcc1[PENDING_ARRAY].Array();
    for (vector<BSONElement>::iterator it = pendingArr.begin() + 1; 
	 it != pendingArr.end(); 
	 ++it){
	TransactionInfo info1;
	makeInfoFromTransaction(*it, info1);
	if (matchRequest(info, info1)) {
	    // Remove pending
	    string accId = subAcc1[OBJ_ID].OID().toString();
	    removePendingTransaction(accId, info1);
	    return true;
	}
    }
    return false;
}

void makeInfoFromTransaction(BSONElement &pending, TransactionInfo &info) {
    info.id0 = getFieldValue(pending, PLAYER0);
    info.id1 = getFieldValue(pending, PLAYER1);
    info.subId0 = getFieldValue(pending, SUBACC0);
    info.subId1 = getFieldValue(pending, SUBACC1);
    info.itemsId0 = getFieldValue(pending, ITEMLIST0);
    info.itemsId1 = getFieldValue(pending, ITEMLIST1);
}

bool matchRequest(const TransactionInfo &sending, 
		  const TransactionInfo &receiving) {
    int result = 0;
    result += sending.id0.compare(receiving.id1);
    result += sending.id1.compare(receiving.id0);
    result += sending.subId0.compare(receiving.subId1);
    result += sending.subId1.compare(receiving.subId0);
    result += sending.itemsId0.compare(receiving.itemsId1);
    result += sending.itemsId1.compare(receiving.itemsId0);
    
    return (result == 0);
}


// Add pending transaction to a sub account
// Here simply put all information in an json object
// e.g. { p0 : id0,
//        p1 : id1,
//        s0 : subId0,
//        s1 : subId1,
//        items0 : itemsId0,
//        items1 : itemsId1,
//        createTime : time
//      }
// $push
// db.collection.update( <query>,
//                      { $push: { <field>: <value> } }
//                     )
void addPendingTransaction(const string &accID, const TransactionInfo &info) {
    BSONObjBuilder query;
    query.append(SUB_ACC_ID_FIELD, OID(accID));
    
    BSONObjBuilder t;
    t.append(PLAYER0, info.id0);
    t.append(PLAYER1, info.id1);
    t.append(SUBACC0, info.subId0);
    t.append(SUBACC1, info.subId1);
    t.append(ITEMLIST0, fromjson(info.itemsId0));
    t.append(ITEMLIST1, fromjson(info.itemsId1));

    BSONObj a = query.obj();
    BSONObj b = t.obj();

    conn->arrayPush(db_ns, a, PENDING_ARRAY, b);
}

void removePendingTransaction(const string &accID, const TransactionInfo &info) {
    BSONObjBuilder query;
    query.append(SUB_ACC_ID_FIELD, OID(accID));

    BSONObjBuilder t;
    t.append(PLAYER0, info.id0);
    t.append(PLAYER1, info.id1);
    t.append(SUBACC0, info.subId0);
    t.append(SUBACC1, info.subId1);
    t.append(ITEMLIST0, fromjson(info.itemsId0));
    t.append(ITEMLIST1, fromjson(info.itemsId1));

    conn->arrayPull(db_ns, query.obj(), PENDING_ARRAY, t.obj());
}

bool hasPendingTransaction(const string &accId, const TransactionInfo &info) {
    BSONObjBuilder query;
    query.append(SUB_ACC_ID_FIELD, OID(accId));
    BSONObjBuilder t;
    t.append(PLAYER0, info.id0);
    t.append(PLAYER1, info.id1);
    t.append(SUBACC0, info.subId0);
    t.append(SUBACC1, info.subId1);
    t.append(ITEMLIST0, fromjson(info.itemsId0));
    t.append(ITEMLIST1, fromjson(info.itemsId1));

    BSONObj b = t.obj();
    query.append(PENDING_ARRAY, b);
    BSONObj a = query.obj();

    BSONObj bson = conn->findOne(db_ns, a);

    return !bson.isEmpty();
}


// Log transaction
// Add complete transaction into player's account if needed
void logTransaction(const TransactionInfo &info) {
    if (!LOG_SELF_TRANSACTION && info.id0.compare(info.id1) == 0) {
	return;
    }
    if (info.id0.compare(info.id1) == 0) {
	// Self transaction
	addCompletedTransaction(info, 0);
	return;
    }
    addCompletedTransaction(info, 0);
    addCompletedTransaction(info, 1);
}

// Add completed transaction into the array under 
// player acount (parent account of the sub account).
//
// $push
// db.collection.update( <query>,
//                      { $push: { <field>: <value> } }
//                     )  
//
// @param flag indicates which id to use. flag = 0 or not 0.
//
void addCompletedTransaction(const TransactionInfo &info, int flag) {
    TransactionInfo tmp = info;
    if (flag != 0) {
	if (isGlobalAccount(info.id1)) {
	    // Do not log transaction for global account
	    return;
	}
	reverseInfo(tmp);
    }
    BSONObjBuilder query;
    query.append(ACC_ID_FIELD, OID(tmp.id0));

    BSONObjBuilder t;
    t.append(PLAYER0, tmp.id0);
    t.append(PLAYER1, tmp.id1);
    t.append(SUBACC0, tmp.subId0);
    t.append(SUBACC1, tmp.subId1);
    t.append(ITEMLIST0, fromjson(tmp.itemsId0));
    t.append(ITEMLIST1, fromjson(tmp.itemsId1));
    t.append(CREATE_TIME, getCurrentTime());

    conn->arrayPush(db_ns, query.obj(), TRANSACTION_HISTORY, t.obj());
}

// Reverse ids in info 
void reverseInfo(TransactionInfo &info) {
    string id = info.id0;
    string subid = info.subId0;
    string items = info.itemsId0;
    info.id0 = info.id1;
    info.subId0 = info.subId1;
    info.itemsId0 = info.itemsId1;
    info.id1 = id;
    info.subId1 = subid;
    info.itemsId1 = items;
}


void executeTransaction(const TransactionInfo &info) {
    BSONObj items0 = fromjson(info.itemsId0);
    BSONObj items1 = fromjson(info.itemsId1);
    
    vector<BSONElement> array = items0.firstElement().Array();
    for (vector<BSONElement>::iterator it = array.begin() + 1;
	 it != array.end();
	 it ++) {
	string itemId(getFieldValue(*it));
	addItemToSubAccount(info.subId1, itemId);
	removeItemFromSubAccount(info.subId0, itemId);
    }
    array = items1.firstElement().Array();
    for (vector<BSONElement>::iterator it = array.begin() + 1;
	 it != array.end();
	 it ++) {
	string itemId(getFieldValue(*it));
	addItemToSubAccount(info.subId0, itemId);
	removeItemFromSubAccount(info.subId1, itemId);
    }
	     
    logTransaction(info);
}

// Update items array
//
// Use $pullAll or $pushAll
// 
// db.collection.update( <query>,
//                      { $pullAll: { <arrayField>: [ <value1>, <value2> ... ] } }
//                    )
// db.collection.update( <query>, 
//                      { $pushAll: { <arrayField>: [ <value1>, <value2> ... ] } }
//                   )
//
void updateItemArray(const string method, const string &accID, 
		     const string &itemIDs) {
    // Not use any more
    BSONObjBuilder query;
    query.append(SUB_ACC_ID_FIELD, accID);
    
    BSONObjBuilder a;
    a.append(ITEM_ARRAY_FIELD, itemIDs);

    BSONObjBuilder b;
    b.append(method, a.obj());
        
    conn->update(db_ns, query.obj(), b.obj());        
}

// Check if id is global id
bool isGlobalAccount(const string &id) {
    BSONObj acc = conn->findOne(db_ns, QUERY(ACC_ID_FIELD << OID(id)));
    
    int type = atoi(getFieldValue(acc, ACC_TYPE_FILED).c_str());
    if (type == TYPE_GLOBAL_ACCOUNT) {
	return true;
    }
    return false;
}

void getAccountInfo(const string &id) {
    BSONObjBuilder query;
    query.append(ACC_ID_FIELD, OID(id));
    BSONObj accInfo = conn->findOne(db_ns, query.obj());
    
    if (accInfo.isEmpty()) {
	error("Account doesn't exist");
    }
    else {
	sendJson(accInfo.jsonString());
    }
}

void getSubAccountInfo(const string &id, const string &subId) {    
    BSONObj subInfo = conn->findOne(db_ns, QUERY(SUB_ACC_ID_FIELD << OID(subId)));
    
    int type = atoi(getFieldValue(subInfo, ACC_TYPE_FILED).c_str());
    if (type == TYPE_SUB_ACCOUNT) {
	string pId = getFieldValue(subInfo, SUB_PARENT_FIELD);
	if (pId.compare(id) != 0) {
	    error("Sub account doesn't belong to account.");
	}
	sendJson(subInfo.jsonString());
	return;
    }
    error("Sub account doesn't exist.");
}

void getTransactionHistory(const string &id) {
    BSONObjBuilder query;
    query.append(ACC_ID_FIELD, id);
    BSONObj accInfo = conn->findOne(db_ns, query.obj());
    
    string history = getFieldValue(accInfo, TRANSACTION_HISTORY);
    
    // TODO: send history back to client
    string result("{\"history\":\"");
    result += history;
    result += "\"}";
    sendJson(result);
}

void ensureAccountType(const BSONObj &acc, int accType) {
    int type = atoi(getFieldValue(acc, ACC_TYPE_FILED).c_str());
    if ((type & accType) == 0) {
	error("Account type error.");
    }
}

void ensureRelationship(const BSONObj &acc0, const BSONObj &acc1, int relation) {
    switch (relation) {
	case PARENT:
	{
	    string id0(acc0[OBJ_ID].OID().toString());
	    string id1(acc1[OBJ_ID].OID().toString());
	    if (id0.compare(getFieldValue(acc1, SUB_PARENT_FIELD)) != 0) {
		error("Sub account doesn't belong to account.");
	    }
	    vector<BSONElement> subArray = acc0[SUB_ACC_ARRAY_FILED].Array();
	    if (!findStringInArray(subArray, id1)) {
		error("Sub account doesn't belong to account.");
	    }	    
	    break;
	}
	case ADMIN:
	{
	    string id0(acc0[OBJ_ID].OID().toString());
	    string id1(acc1[OBJ_ID].OID().toString());
	    vector<BSONElement> groupArray = acc0[GROUP_ACC_ARRAY_FIELD].Array();
	    if (!findStringInArray(groupArray, id1)) {
		error("Account doesn't belong to this group.");
	    }
	    vector<BSONElement> adminArray = acc1[ADMIN_ARRAY_FIELD].Array();
	    if (!findStringInArray(adminArray, id0)) {
		error("Account is not admin.");
	    }
	    break;
	}
	case MEMBER:
	{
	    string id0(acc0[OBJ_ID].OID().toString());
	    string id1(acc1[OBJ_ID].OID().toString());
	    vector<BSONElement> groupArray = acc0[GROUP_ACC_ARRAY_FIELD].Array();
	    if (!findStringInArray(groupArray, id1)) {
		error("Account doesn't belong to this group.");
	    }
	    bool flag = false;
	    vector<BSONElement> adminArray = acc1[ADMIN_ARRAY_FIELD].Array();
	    flag = findStringInArray(adminArray, id0);
	    if (!flag) {
		vector<BSONElement> memberArray = acc1[MEMBER_ARRAY_FIELD].Array();
		flag = findStringInArray(memberArray, id0);
	    }
	    if (!flag) {
		error("Group doesn't have this member.");		
	    }
	    break;	    
	}
    }
}

void ensureGroupRelations(const string &gid, 
			  const string &id0, const string &id1) {
    BSONObj group = conn->findOne(db_ns, QUERY(GROUP_ID_FIELD << OID(gid)));
    BSONObj acc0 = conn->findOne(db_ns, QUERY(ACC_ID_FIELD << OID(id0)));
    BSONObj acc1 = conn->findOne(db_ns, QUERY(ACC_ID_FIELD << OID(id1)));
    
    ensureAccountType(group, TYPE_GROUP_ACCOUNT);
    ensureAccountType(acc0, TYPE_ACCOUNT);
    ensureAccountType(acc1, TYPE_ACCOUNT);
    // Check permission
    ensureRelationship(acc0, group, ADMIN);
    ensureRelationship(acc1, group, MEMBER);
}

bool findStringInArray(vector<BSONElement> &array, const string &str) {
    for (vector<BSONElement>::iterator it = array.begin() + 1; 
	 it != array.end(); 
	 ++it){
	string value = getFieldValue(*it);
	if (value.compare(str) == 0) {
	    return true;
	}
    }
    return false;
}

// Create a new account
void addAccount(const string &username) {
    BSONObjBuilder acc;
    acc.genOID();
    acc.append(USER_NAME_FIELD, username);
    acc.append(ACC_TYPE_FILED, TYPE_ACCOUNT);
    acc.append(SUB_ACC_ARRAY_FILED, EMPTY_JSON_ARRAY);
    acc.append(GROUP_ACC_ARRAY_FIELD, EMPTY_JSON_ARRAY);
    acc.append(TRANSACTION_HISTORY, EMPTY_JSON_ARRAY);
    BSONObj obj = acc.obj();
    string oid(obj["_id"].OID().toString());

    conn->insert(db_ns, obj);
    // add account oid into user root
    conn->arrayPush(db_ns, BSON(ROOT_TYPE_FIELD << USER_ROOT), 
		    CHILD_ARRAY_FILED, oid);

    BSONObjBuilder result;
    result.append("message", "Added account.");
    result.append(OBJ_ID, oid);
    sendJson(result.obj().jsonString());
}

void removeAccount(const string &id) {
    BSONObj accInfo = conn->findOne(db_ns, QUERY(ACC_ID_FIELD << OID(id)));

    if (accInfo.isEmpty()) {
	error("Account doesn't exist.");
    }

    BSONElement subArray = accInfo[SUB_ACC_ARRAY_FILED];
    
    if (subArray.ok()) {
	vector<BSONElement> elem = subArray.Array();
	for (vector<BSONElement>::iterator it = elem.begin() + 1; 
	     it != elem.end(); 
	     ++it){
	    string subId = (*it)[OBJ_ID].OID().toString();

	    clearSubAccount(subId);
	    
	    conn->remove(db_ns, QUERY(SUB_ACC_ID_FIELD << OID(subId)));
	}
    }

    conn->arrayPull(db_ns, BSON(ROOT_TYPE_FIELD << USER_ROOT),
		    CHILD_ARRAY_FILED, id);

    conn->remove(db_ns, QUERY(ACC_ID_FIELD << OID(id)));
    
    BSONObjBuilder result;
    result.append("message", "Removed account.");
    result.append(OBJ_ID, id);
    sendJson(result.obj().jsonString());
}

void addGlobalAccount(const string &name) {
    BSONObjBuilder acc;
    acc.genOID();
    acc.append(USER_NAME_FIELD, name);
    acc.append(ACC_TYPE_FILED, TYPE_GLOBAL_ACCOUNT);
    acc.append(SUB_ACC_ARRAY_FILED, EMPTY_JSON_ARRAY);
    acc.append(TRANSACTION_HISTORY, EMPTY_JSON_ARRAY);
    BSONObj obj = acc.obj();
    string oid(obj["_id"].OID().toString());

    conn->insert(db_ns, obj);
    // add global account oid into global root
    conn->arrayPush(db_ns, BSON(ROOT_TYPE_FIELD << GLOBAL_ROOT), 
		    CHILD_ARRAY_FILED, oid);

    BSONObjBuilder result;
    result.append("message", "Added global account.");
    result.append(OBJ_ID, oid);
    sendJson(result.obj().jsonString());    
}
void removeGlobalAccount(const string &id) {
    BSONObj accInfo = conn->findOne(db_ns, QUERY(ACC_ID_FIELD << OID(id)));

    if (accInfo.isEmpty()) {
	error("Account doesn't exist.");
    }

    BSONElement subArray = accInfo[SUB_ACC_ARRAY_FILED];
    
    if (subArray.ok()) {
	vector<BSONElement> elem = subArray.Array();
	for (vector<BSONElement>::iterator it = elem.begin() + 1; 
	     it != elem.end(); 
	     ++it){
	    string subId = (*it)[OBJ_ID].OID().toString();

	    clearSubAccount(subId);
	    
	    conn->remove(db_ns, QUERY(SUB_ACC_ID_FIELD << OID(subId)));
	}
    }

    conn->arrayPull(db_ns, BSON(ROOT_TYPE_FIELD << GLOBAL_ROOT),
		    CHILD_ARRAY_FILED, id);

    conn->remove(db_ns, QUERY(ACC_ID_FIELD << OID(id)));
    
    BSONObjBuilder result;
    result.append("message", "Removed global account.");
    result.append(OBJ_ID, id);
    sendJson(result.obj().jsonString());    
}


// Create a new sub account
void addSubAccount(const string &id) {
    BSONObj acc = conn->findOne(db_ns, QUERY(ACC_ID_FIELD << OID(id)));
    
    ensureAccountType(acc, TYPE_ACCOUNT | TYPE_GLOBAL_ACCOUNT);    
    
    // Create a sub account
    BSONObjBuilder sub;
    sub.genOID();    
    sub.append(ACC_TYPE_FILED, TYPE_SUB_ACCOUNT);
    sub.append(SUB_PARENT_FIELD, id);
    sub.append(ITEM_ARRAY_FIELD, EMPTY_JSON_ARRAY);
    sub.append(PENDING_ARRAY, EMPTY_JSON_ARRAY);
    BSONObj obj = sub.obj();
    conn->insert(db_ns, obj);
    
    // Insert subid into sub acc array
    string subId(obj["_id"].OID().toString());

    conn->arrayPush(db_ns, BSON(ACC_ID_FIELD << OID(id)), SUB_ACC_ARRAY_FILED, subId);
    
    BSONObjBuilder result;
    result.append("message", "Added sub account");
    result.append(OBJ_ID, subId);
    sendJson(result.obj().jsonString());
}

// Remove a sub account from account
void removeSubAccount(const string &id, const string &subId) {
    BSONObj accInfo = conn->findOne(db_ns, QUERY(ACC_ID_FIELD << OID(id)));
    BSONObj subInfo = conn->findOne(db_ns, QUERY(SUB_ACC_ID_FIELD << OID(subId)));
    
    ensureAccountType(accInfo, TYPE_ACCOUNT | TYPE_GLOBAL_ACCOUNT);
    ensureAccountType(subInfo, TYPE_SUB_ACCOUNT);
    ensureRelationship(accInfo, subInfo, PARENT);
    
    // remove sub account from database
    clearSubAccount(subId);
    conn->remove(db_ns, QUERY(SUB_ACC_ID_FIELD << OID(subId)));
    // remove from user's group array
    conn->arrayPull(db_ns, BSON(ACC_ID_FIELD << OID(id)),
		    SUB_ACC_ARRAY_FILED, subId);

    BSONObjBuilder result;
    result.append("message", "Removed sub account");
    result.append(OBJ_ID, subId);
    sendJson(result.obj().jsonString());
}

void clearSubAccount(const string &subId) {
    BSONObj subAcc = conn->findOne(db_ns, QUERY(SUB_ACC_ID_FIELD <<  OID(subId)));
    
    if (!subAcc.isEmpty()) {
	vector<BSONElement> itemArr = subAcc[ITEM_ARRAY_FIELD].Array();
	for (vector<BSONElement>::iterator it = itemArr.begin() + 1; 
	     it != itemArr.end(); 
	     ++it){
	    string itemId = getFieldValue(*it);
	    conn->remove(db_ns, QUERY(OBJ_ID << OID(itemId)));
	}
    }
}

// Create a new group account, using user id as the first admin
void createGroup(const string &id) {
    BSONObjBuilder group;
    group.genOID();
    group.append(ACC_TYPE_FILED, TYPE_GROUP_ACCOUNT);
    group.append(ADMIN_ARRAY_FIELD, BSON_ARRAY("array" << id));
    group.append(MEMBER_ARRAY_FIELD, EMPTY_JSON_ARRAY);

    BSONObj obj = group.obj();
    conn->insert(db_ns, obj);

    // Insert group id into group array
    string gid(obj["_id"].OID().toString());

    conn->arrayPush(db_ns, BSON(ACC_ID_FIELD << OID(id)), GROUP_ACC_ARRAY_FIELD, gid);

    // Insert group id into group root
    conn->arrayPush(db_ns, BSON(ROOT_TYPE_FIELD << GROUP_ROOT), 
		    CHILD_ARRAY_FILED, gid);

    BSONObjBuilder result;
    result.append("message", "Created group.");
    result.append(OBJ_ID, gid);
    sendJson(result.obj().jsonString());
}

void destroyGroup(const string &gid) {
    BSONObj group = conn->findOne(db_ns, QUERY(GROUP_ID_FIELD << OID(gid)));
    if (group.isEmpty()) {
	error("Group doesn't exist.");
    }
    
    ensureAccountType(group, TYPE_GROUP_ACCOUNT);

    BSONElement accounts;
    accounts = group[MEMBER_ARRAY_FIELD];
    if (accounts.ok()) {
	vector<BSONElement> memberArr = accounts.Array();
	for (vector<BSONElement>::iterator it = memberArr.begin() + 1; 
	     it != memberArr.end(); 
	     ++it){
	    string id(getFieldValue(*it));
	    conn->arrayPull(db_ns, BSON(ACC_ID_FIELD << OID(id)), 
			    GROUP_ACC_ARRAY_FIELD, gid);
	}
    }
    accounts = group[ADMIN_ARRAY_FIELD];
    if (accounts.ok()) {
	vector<BSONElement> adminArr = accounts.Array();
	for (vector<BSONElement>::iterator it = adminArr.begin() + 1; 
	     it != adminArr.end();
	     ++it){
	    string id(getFieldValue(*it));
	    conn->arrayPull(db_ns, BSON(ACC_ID_FIELD << OID(id)), 
			    GROUP_ACC_ARRAY_FIELD, gid);
	}
    }

    // remove group id from group root
    conn->arrayPull(db_ns, BSON(ROOT_TYPE_FIELD << GROUP_ROOT),
		    CHILD_ARRAY_FILED, gid);
    
    conn->remove(db_ns, QUERY(GROUP_ID_FIELD << OID(gid)));

    BSONObjBuilder result;
    result.append("message", "Destroyed group.");
    result.append(OBJ_ID, gid);
    sendJson(result.obj().jsonString());
}

// Add group member
void addGroupMember(const string &gid, const string &id0, const string &id1) {
    ensureGroupRelations(gid, id0, id1);

    conn->arrayPush(db_ns, BSON(GROUP_ID_FIELD << OID(gid)),
		    MEMBER_ARRAY_FIELD, id1);
    
    conn->arrayPush(db_ns, BSON(ACC_ID_FIELD << OID(id1)),
		    GROUP_ACC_ARRAY_FIELD, gid);

    BSONObjBuilder result;
    result.append("message", "Added member to group.");
    result.append("Group id", gid);
    result.append("Admin id", id0);
    result.append("Member id", id1);
    sendJson(result.obj().jsonString());
}

void removeGroupMember(const string &gid, const string &id0, const string &id1) {
    ensureGroupRelations(gid, id0, id1);

    conn->arrayPull(db_ns, BSON(GROUP_ID_FIELD << OID(gid)), MEMBER_ARRAY_FIELD, id1);
    conn->arrayPull(db_ns, BSON(GROUP_ID_FIELD << OID(gid)), ADMIN_ARRAY_FIELD, id1);
   
    // remove from user's group array
    conn->arrayPull(db_ns, BSON(ACC_ID_FIELD << OID(id1)), GROUP_ACC_ARRAY_FIELD, gid);

    BSONObjBuilder result;
    result.append("message", "Removed member from group.");
    result.append("Group id", gid);
    result.append("Admin id", id0);
    result.append("Member id", id1);
    sendJson(result.obj().jsonString());
}

void changeGroupPermission(const string &gid, 
			   const string &id0, const string &id1,
			   int permission) {
    ensureGroupRelations(gid, id0, id1);
    
    BSONObj query = BSON(GROUP_ID_FIELD << OID(gid));
    
    BSONObjBuilder result;
    switch(permission) {
    case 1:
	conn->arrayPull(db_ns, query, MEMBER_ARRAY_FIELD, id1);
	conn->arrayPush(db_ns, query, ADMIN_ARRAY_FIELD, id1);
	result.append("message", "Permission changed to admin.");
	break;
    case 0:
    default:
	conn->arrayPush(db_ns, query, MEMBER_ARRAY_FIELD, id1);
	conn->arrayPull(db_ns, query, ADMIN_ARRAY_FIELD, id1);
	result.append("message", "Permission changed to member.");
	break;
    }

    result.append("Group id", gid);
    result.append("Admin id", id0);
    result.append("Member id", id1);
    sendJson(result.obj().jsonString());
}

void addItemToSubAccount(const string &subId, const string &itemId) {
    conn->arrayPush(db_ns, BSON(SUB_ACC_ID_FIELD << OID(subId)),
		    ITEM_ARRAY_FIELD, itemId);
}

void removeItemFromSubAccount(const string &subId, const string &itemId) {
    conn->arrayPull(db_ns, BSON(SUB_ACC_ID_FIELD << OID(subId)),
		    ITEM_ARRAY_FIELD, itemId);
}

// Send back error message
void error(const string &msg) {
    cout << "{\"status\":\"error\",\"message\":\"" << msg << "\"}\n";
    exit(0);
}

void sendJson(const string &jsonString) {
    cout << "{\"status\":\"success\",\"content\":" << jsonString << "}\n";
}

void initTreeStruct(bool force) {
    if (force) {
	conn->remove(db_ns, "");
    }
    else {
	BSONObj root = conn->findOne(db_ns, QUERY(ROOT_TYPE_FIELD << TRANSACTION_ROOT));
	if (!root.isEmpty()) {
	    error("Tree root already exists.");
	}
    }
    BSONObjBuilder userRootBuilder;
    userRootBuilder.genOID();
    userRootBuilder.append(ROOT_TYPE_FIELD, USER_ROOT);
    userRootBuilder.append(CHILD_ARRAY_FILED, EMPTY_JSON_ARRAY);    
    BSONObj userRoot = userRootBuilder.obj();
    
    BSONObjBuilder groupRootBuilder;
    groupRootBuilder.genOID();
    groupRootBuilder.append(ROOT_TYPE_FIELD, GROUP_ROOT);
    groupRootBuilder.append(CHILD_ARRAY_FILED, EMPTY_JSON_ARRAY);    
    BSONObj groupRoot = groupRootBuilder.obj();
    
    BSONObjBuilder globalRootBuilder;
    globalRootBuilder.genOID();
    globalRootBuilder.append(ROOT_TYPE_FIELD, GLOBAL_ROOT);
    globalRootBuilder.append(CHILD_ARRAY_FILED, EMPTY_JSON_ARRAY);
    BSONObj globalRoot = globalRootBuilder.obj();
    
    BSONObjBuilder treeRootBuilder;
    treeRootBuilder.append(ROOT_TYPE_FIELD, TRANSACTION_ROOT);
    treeRootBuilder.append(USER_ROOT, userRoot["_id"].OID().toString());
    treeRootBuilder.append(GROUP_ROOT, groupRoot["_id"].OID().toString());
    treeRootBuilder.append(GLOBAL_ROOT, globalRoot["_id"].OID().toString());
    
    BSONObj treeRoot = treeRootBuilder.obj();

    conn->insert(db_ns, userRoot);
    conn->insert(db_ns, groupRoot);
    conn->insert(db_ns, globalRoot);
    conn->insert(db_ns, treeRoot);
    
    sendJson(treeRoot.jsonString());
}

void ensureInitTree() {
    BSONObj root = conn->findOne(db_ns, QUERY(ROOT_TYPE_FIELD << TRANSACTION_ROOT));
    if (root.isEmpty()) {
	error("Transaction tree streucture not initialized.");
    }    
}

void getTreeRootInfo() {
    BSONObj root = conn->findOne(db_ns, QUERY(ROOT_TYPE_FIELD << TRANSACTION_ROOT));
    sendJson(root.jsonString());
}
void getUserRootInfo() {
    BSONObj userRoot = conn->findOne(db_ns, QUERY(ROOT_TYPE_FIELD << USER_ROOT));  
    sendJson(userRoot.jsonString());    
}
void getGroupRootInfo() {
    BSONObj groupRoot = conn->findOne(db_ns, QUERY(ROOT_TYPE_FIELD << GROUP_ROOT));  
    sendJson(groupRoot.jsonString());    
}
void getGlobalRootInfo() {
    BSONObj globalRoot = conn->findOne(db_ns, QUERY(ROOT_TYPE_FIELD << GLOBAL_ROOT));
    sendJson(globalRoot.jsonString());
}

