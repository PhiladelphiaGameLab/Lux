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
#include "../mongodb/MongoWrapper.h"
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
    ACCOUNT,
    SUB_ACCOUNT,
    GLOBAL_ACCOUNT,
    GROUP_ACCOUNT
};

enum AccRelation {
    PARENT,
    MEMBER,
    ADMIN,    
};


// Account structure
// e.g.
// {
//  "_id" : "xxx", (created by mongo)
//  "name" : username,
//  "type" : ACCOUNT, or GLOBAL_ACCOUNT,
//  "group" : [],
//  "sub_acc" : [],
//  "trans_history" : []
//  }
// the field name for account id
const string ACC_ID_FIELD("_id"); 
// filed name for username in account document
const string USER_NAME_FIELD("name");
// filed name for group account
const string GROUP_ACC_FIELD("group");
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
const string GROUP_ID("_id");
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
bool checkItemIDs(const BSONObj &subaccount, const BSONElement &items);
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
bool findStringInArray(vector<BSONElement> &array, const string &str);
void addAccount(const string &username);
void removeAccount(const string &id);
void addSubAccount(const string &id);
void removeSubAccount(const string &id, const string &subId);
void clearSubAccount(const string &subId);
void createGroup(const string &id);
void destoryGroup(const string &gid);
void addGroupMember(const string &gid, const string &id);
void removeGroupMember(const string &gid, const string &id);
void changeGroupPermission(const string &gid, const string &id, int permission);
void addItemToSubAccount(const string &subAccId, const string &itemId);
void removeItemFromSubAccount(const string &subAccId, const string &itemId);
void error(const string &msg);
void sendJson(const string &jsonString);

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
    return (time_cnt < TOTAL_TIME);
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

    if(method.compare("transaction") == 0) {
        string id0 = environment->get("id0"); //verify id0 == EUID or use EUID directly?
        string id1 = environment->get("id1");
        string subId0 = environment->get("subId0");
	string subId1 = environment->get("subId1");
        string itemsId0 = environment->get("itemsId0");
	string itemsId1 = environment->get("itemsId1");
	
	TransactionInfo info(id0, id1,
			     subId0, subId1,
			     itemsId0, itemsId1);	
	BSONObj subAcc0, subAcc1;
	
	findSubAcc(info, subAcc0, subAcc1);
	
        if (!checkItemIDs(subAcc0, (fromjson(info.itemsId0)["items"]))) {
            //failed error
	    error("Items don't match.");
        }
	if (!checkItemIDs(subAcc1, (fromjson(info.itemsId1)["items"]))) {
	    error("Items don't match.");
	}
	
	if (subId0.compare(subId1) < 0) {
	    // Always let the bigger one do check match stuff
	    string accID = subId0;
	    addPendingTransaction(accID, info);
	    // Sleep for some time and cancel pending this transaction
	    this_thread::sleep_for(chrono::seconds(TOTAL_TIME * 2));
	    // Remove pending transaction when time out
	    // If this pending transaction is already been processed
	    // this function will do nothing.	    	    
	    if (hasPendingTransaction(accID, info)) {
		removePendingTransaction(accID, info);
		string result = "{message:\"Transaction successfully finished.\"}";
		sendJson(result);
	    }
	    else {
		error("Transaction failed.");
	    }
	    return 0;
	}
	
	// The bigger one will check matched pending transaction 
	// several times untill time out
	while (!timedOut()) {
	    // Get the lastest subAcc1
	    subAcc1 = conn->findOne(db_ns, QUERY(SUB_ACC_ID_FIELD << subId1));

	    if (checkMatch(info, subAcc0, subAcc1)) {
		executeTransaction(info);
		string result = "{message:\"Transaction successfully finished.\"}";
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
	destoryGroup(gid);
    }
    else if (method.compare("addGroupMember") == 0) {
	string id = environment->get("id0");
	string gid = environment->get("groupId");
	addGroupMember(gid, id);
    }
    else if (method.compare("removeGroupMember") == 0) {
	string id = environment->get("id0");
	string gid = environment->get("groupId");
	removeGroupMember(gid, id);
    }
    else if (method.compare("changeGroupPermission") == 0) {
	string id = environment->get("id0");
	string gid = environment->get("groupId");
	string permission = environment->get("permission");
	changeGroupPermission(gid, id, atoi(permission.c_str()));
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
    BSONObj acc = conn->findOne(db_ns, QUERY(ACC_ID_FIELD << info.id0));
    
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
    acc = conn->findOne(db_ns, QUERY(ACC_ID_FIELD << info.id1));
    if (findSubAccInAcc(acc, info.subId1, subAcc1) == false) {
	return false;
    }
    
    return true;
}

// Find a subaccount in account
// Here I made the assumption that in an account object
// subaccounts' ids are stored in an array.
// e.g. Account: {"_id":xxxx, "sub_acc_array":[xxx,xxx,xxx,xxx]}.
bool findSubAccInAcc(const BSONObj &acc, const string &index, BSONObj &subAcc) {
    /*
    BSONElement subArray = acc[SUB_ACC_ARRAY_FILED];
    if (!subArray.ok()) {
	// If account doesn't have sub accounts, return false
	return false;
    }
    BSONElement id = subArray[index];
    if (!id.ok()) {
	// If sub account array doesn't have this sub account index, return false
	return false;
    }
    
    string subId = getFieldValue(subArray, index);

    // use this sub id to find sub account in the db
    subAcc = conn->findOne(db_ns, QUERY(SUB_ACC_ID_FIELD << OID(subid)));
    */
    return true;
}

// Check if all items are in a subaccount.
// Return flase if a least one is missing.
//
// Assuming item ids in sub account are stored in an array
// e.g. {sub_acc_id : xxx, items : [xxx,xxx,xxx]}
bool checkItemIDs(const BSONObj &subaccount, const BSONElement &items) {
    int i = 0, j;
    vector<BSONElement> itemArr0 = items.Array();
    vector<BSONElement> itemArr1 = subaccount[ITEM_ARRAY_FIELD].Array();
    for (vector<BSONElement>::iterator it0 = itemArr0.begin() + 1; 
	 it0 != itemArr0.end(); 
	 ++it0){
	string value0((*it0).toString(false));
	bool find = false;
	for (vector<BSONElement>::iterator it1 = itemArr1.begin() + 1; 
	     it1 != itemArr1.end(); 
	     ++it1){
	    string value1((*it1).toString(false));
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
    if (info.id0.compare(info.id1) == 0) {
	// Self transaction
	return true;
    }
    
    if (isGlobalAccount(info.id1)) {
	return true;
    }
    
    vector<BSONElement> pendingArr = subAcc1[PENDING_ARRAY].Array();
    for (vector<BSONElement>::iterator it = pendingArr.begin() + 1; 
	 it != pendingArr.end(); 
	 ++it){
	TransactionInfo info1;
	makeInfoFromTransaction(*it, info1);
	if (matchRequest(info, info1)) {
	    // Remove pending
	    string accId = getFieldValue(subAcc0, SUB_ACC_ARRAY_FILED);
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
    query.append(SUB_ACC_ID_FIELD, accID);

    BSONObjBuilder t;
    t.append(PLAYER0, info.id0);
    t.append(PLAYER1, info.id1);
    t.append(SUBACC0, info.subId0);
    t.append(SUBACC1, info.subId1);
    t.append(ITEMLIST0, info.itemsId0);
    t.append(ITEMLIST1, info.itemsId1);
    t.append(CREATE_TIME, getCurrentTime());
    BSONObjBuilder p;
    p.append(PENDING_ARRAY, t.obj());
    BSONObjBuilder q;
    q.append("$push", p.obj());
    
    conn->update(db_ns, query.obj(), q.obj());
}

void removePendingTransaction(const string &accID, const TransactionInfo &info) {
    BSONObjBuilder query;
    query.append(SUB_ACC_ID_FIELD, accID);

    BSONObjBuilder t;
    t.append(PLAYER0, info.id0);
    t.append(PLAYER1, info.id1);
    t.append(SUBACC0, info.subId0);
    t.append(SUBACC1, info.subId1);
    t.append(ITEMLIST0, info.itemsId0);
    t.append(ITEMLIST1, info.itemsId1);
    BSONObjBuilder p;
    p.append(PENDING_ARRAY, t.obj());
    BSONObjBuilder q;
    q.append("$pull", p.obj());
    
    conn->update(db_ns, query.obj(), q.obj());
}

bool hasPendingTransaction(const string &accId, const TransactionInfo &info) {
    BSONObjBuilder query;
    query.append(ACC_ID_FIELD, accId);
    BSONObjBuilder t;
    t.append(PLAYER0, info.id0);
    t.append(PLAYER1, info.id1);
    t.append(SUBACC0, info.subId0);
    t.append(SUBACC1, info.subId1);
    t.append(ITEMLIST0, info.itemsId0);
    t.append(ITEMLIST1, info.itemsId1);
    query.append(PENDING_ARRAY, t.obj());
    BSONObj bson = conn->findOne(db_ns, query.obj());
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
    query.append(ACC_ID_FIELD, tmp.id0);

    BSONObjBuilder t;
    t.append(PLAYER0, tmp.id0);
    t.append(PLAYER1, tmp.id1);
    t.append(SUBACC0, tmp.subId0);
    t.append(SUBACC1, tmp.subId1);
    t.append(ITEMLIST0, tmp.itemsId0);
    t.append(ITEMLIST1, tmp.itemsId1);
    t.append(CREATE_TIME, getCurrentTime());
    BSONObjBuilder p;
    p.append(TRANSACTION_HISTORY, t.obj());
    BSONObjBuilder q;
    q.append("$push", p.obj());
    
    conn->update(db_ns, query.obj(), q.obj());
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
    updateItemArray("$pullAll", info.subId0, info.itemsId0);
    updateItemArray("$pullAll", info.subId1, info.itemsId1);
    updateItemArray("$pushAll", info.subId0, info.itemsId1);
    updateItemArray("$pushAll", info.subId1, info.itemsId0);
    removePendingTransaction(info.subId1, info);
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
    // TODO check is id is global id
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
    if (type == SUB_ACCOUNT) {
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
    string result("{message:\"");
    result += history;
    result += "\"}";
    sendJson(result);
}

void ensureAccountType(const BSONObj &acc, int accType) {
    int type = atoi(getFieldValue(acc, ACC_TYPE_FILED).c_str());
    if (type != accType) {
	error("Account type error");
    }
}

void ensureRelationship(const BSONObj &acc0, const BSONObj &acc1, int relation) {
    switch (relation) {
	case PARENT:
	    string id0 = acc0[OBJ_ID].OID().toString();
	    string id1 = acc1[OBJ_ID].OID().toString();
	    if (id0.compare(getFieldValue(acc1, SUB_PARENT_FIELD)) != 0) {
		error("Sub account doesn't belong to account.");
	    }
	    vector<BSONElement> subArray = acc0[SUB_ACC_ARRAY_FILED].Array();
	    if (!findStringInArray(subArray, id1)) {
		error("Sub account doesn't belong to account.");
	    }	    
	    break;
    }
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
    acc.append(ACC_TYPE_FILED, ACCOUNT);
    acc.append(SUB_ACC_ARRAY_FILED, EMPTY_JSON_ARRAY);
    acc.append(GROUP_ACC_FIELD, EMPTY_JSON_ARRAY);
    acc.append(TRANSACTION_HISTORY, EMPTY_JSON_ARRAY);
    BSONObj obj = acc.obj();

    conn->insert(db_ns, obj);

    BSONObjBuilder result;
    result.append("message", "Added account.");
    result.append(obj[OBJ_ID]);
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

    conn->remove(db_ns, QUERY(ACC_ID_FIELD << OID(id)));
    
    BSONObjBuilder result;
    result.append("message", "Removed account.");
    result.append(OBJ_ID, OID(id));
    sendJson(result.obj().jsonString());
}

// Create a new sub account
void addSubAccount(const string &id) {
    // Create a sub account
    BSONObjBuilder sub;
    sub.genOID();    
    sub.append(ACC_TYPE_FILED, SUB_ACCOUNT);
    sub.append(SUB_PARENT_FIELD, id);
    sub.append(ITEM_ARRAY_FIELD, EMPTY_JSON_ARRAY);
    sub.append(PENDING_ARRAY, EMPTY_JSON_ARRAY);
    BSONObj obj = sub.obj();
    conn->insert(db_ns, obj);
    
    // Insert subid into sub acc array
    string subId(obj["_id"].OID().toString());

    BSONObjBuilder query;
    query.append(ACC_ID_FIELD, OID(id));

    conn->arrayPush(db_ns, query.obj(), SUB_ACC_ARRAY_FILED, subId);
    
    BSONObjBuilder result;
    result.append("message", "Added sub account");
    result.append(OBJ_ID, subId);
    sendJson(result.obj().jsonString());
}

// Remove a sub account from account
void removeSubAccount(const string &id, const string &subId) {
    BSONObj accInfo = conn->findOne(db_ns, QUERY(ACC_ID_FIELD << OID(id)));
    BSONObj subInfo = conn->findOne(db_ns, QUERY(SUB_ACC_ID_FIELD << OID(subId)));
    
    ensureAccountType(accInfo, ACCOUNT);
    ensureAccountType(subInfo, SUB_ACCOUNT);
    ensureRelationship(accInfo, subInfo, PARENT);
    
    // remove sub account from database
    //clearSubAccount(subId);
    conn->remove(db_ns, QUERY(SUB_ACC_ID_FIELD << OID(subId)));
    // remove from user's group array

    // bug here array pull
    conn->arrayPull(db_ns, QUERY(ACC_ID_FIELD << OID(id)),
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
	    string itemId((*it)[OBJ_ID].OID().toString());
	    conn->remove(db_ns, QUERY(OBJ_ID << itemId));
	}
    }
}

// Create a new group account, using user id as the first admin
void createGroup(const string &id) {
    BSONObjBuilder group;
    group.genOID();
    group.append(ACC_TYPE_FILED, GROUP_ACCOUNT);
    group.append(ADMIN_ARRAY_FIELD, BSON_ARRAY("array" << id));
    group.append(MEMBER_ARRAY_FIELD, EMPTY_JSON_ARRAY);

    BSONObj obj = group.obj();
    conn->insert(db_ns, obj);

    // Insert group id into group array
    string gid(obj["_id"].OID().toString());

    BSONObjBuilder query;
    query.append(ACC_ID_FIELD, id);
    conn->arrayPush(db_ns, query.obj(), GROUP_ACC_FIELD, gid);

    string result("{message:\"Created Group\"}");
    sendJson(result);
}

void destoryGroup(const string &gid) {
    BSONObj group = conn->findOne(db_ns, QUERY(GROUP_ID << gid));
    if (group.isEmpty()) {
	error("Group doesn't exist.");
    }
    
    BSONElement accounts;
    accounts = group[MEMBER_ARRAY_FIELD];
    if (accounts.ok()) {
	vector<BSONElement> memberArr = accounts[ITEM_ARRAY_FIELD].Array();
	for (vector<BSONElement>::iterator it = memberArr.begin() + 1; 
	     it != memberArr.end(); 
	     ++it){
	    BSONElement id = (*it)[OBJ_ID];
	    conn->arrayPull(db_ns, BSON(ACC_ID_FIELD << id), GROUP_ACC_FIELD, gid);
	}
    }
    accounts = group[ADMIN_ARRAY_FIELD];
    if (accounts.ok()) {
	vector<BSONElement> adminArr = accounts[ITEM_ARRAY_FIELD].Array();
	for (vector<BSONElement>::iterator it = adminArr.begin() + 1; 
	     it != adminArr.end(); 
	     ++it){
	    BSONElement id = (*it)[OBJ_ID];
	    conn->arrayPull(db_ns, BSON(ACC_ID_FIELD << id), GROUP_ACC_FIELD, gid);
	}
    }
    
    conn->remove(db_ns, QUERY(GROUP_ID << gid));
}

// Add group member
void addGroupMember(const string &gid, const string &id) {
    BSONObjBuilder query;
    query.append(GROUP_ID, gid);
    
    conn->arrayPush(db_ns, query.obj(), MEMBER_ARRAY_FIELD, id);
    
    string result("{message:\"Added Group Member\"}");
    sendJson(result);
}

void removeGroupMember(const string &gid, const string &id) {
    BSONObjBuilder query;
    query.append(GROUP_ID, gid);
    
    conn->arrayPull(db_ns, query.obj(), MEMBER_ARRAY_FIELD, id);
    conn->arrayPull(db_ns, query.obj(), ADMIN_ARRAY_FIELD, id);
   
    // remove from user's group array
    conn->arrayPull(db_ns, BSON(ACC_ID_FIELD << id), GROUP_ACC_FIELD, gid);

    string result("{message:\"Removed Group Member\"}");
    sendJson(result);
}

void changeGroupPermission(const string &gid, const string &id, int permission) {
    BSONObjBuilder query;
    query.append(GROUP_ID, gid);

    switch(permission) {
    case 1:
	conn->arrayPull(db_ns, query.obj(), MEMBER_ARRAY_FIELD, id);
	conn->arrayPush(db_ns, query.obj(), ADMIN_ARRAY_FIELD, id);
	break;
    case 0:
    default:
	conn->arrayPush(db_ns, query.obj(), MEMBER_ARRAY_FIELD, id);
	conn->arrayPull(db_ns, query.obj(), ADMIN_ARRAY_FIELD, id);
	break;
    }
    string result("{message:\"Changed permission\"}");
    sendJson(result);
}

void addItemToSubAccount(const string &subAccId, const string &itemId) {
    conn->arrayPush(db_ns, QUERY(SUB_ACC_ID_FIELD << subAccId), 
		    ITEM_ARRAY_FIELD, itemId);
}

void removeItemFromSubAccount(const string &subAccId, const string &itemId) {
    conn->arrayPull(db_ns, QUERY(SUB_ACC_ID_FIELD << subAccId), 
		    ITEM_ARRAY_FIELD, itemId);
}

// Send back error message
void error(const string &msg) {
    cout << "{status:\"error\",message:\"" << msg << "\"}\n";
    exit(0);
}

void sendJson(const string &jsonString) {
    cout << "{status:\"success\",content:" << jsonString << "}\n";
}
