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

#include "Authenticate.h"
#include "mongoTransaction.h"
#include "mongo/bson/bson.h"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace std;
using namespace mongo;

// Transaction info
// 
// items id should be in the form {items : [xxx,xxx,xxx]}
struct TransactionInfo
{
    string id0; 
    string id1;
    string subaccountId0;
    string subaccountId1;
    string itemsId0;
    string itemsId1;
    TransactionInfo(string i0, string i1,
                    string si0, string si1,
                    string iti0, string iti1):
        id0(i0), id1(i1), subaccountId0(si0), subaccountId1(si1),
        itemsId0(iti0), itemsId1(iti1) {}
};

// global varibales

// get parameters from the HTTP Request
CGI environment;    

// connection to mongo db
MongoWrapper conn("localhost"); 
// the namespace of collection
const string db_ns("db.transaction"); 
// the field name for account id
const string ACC_ID_FIELD("_id"); 
// field name for subaccount, default is equal to account id
const string SUB_ACC_ID_FIELD(ACC_ID_FIELD);
// items array in sub account
const string ITEM_ARRAY_FIELD("items");
// sub account array in account
const string SUB_ACC_ARRAY_FILED("sub_acc_array");
// transaction history array in account
const string TRANSACTION_HISTORY("completed_array");
// flag indicates whether to log self transaction
const bool LOG_SELF_TRANSACTION = false;

// Transaction log object fields:
// e.g.
// {
//   player0 : id0,
//   player1 : id1,
//   subacc0 : subid0,
//   subacc1 : subid1,
//   itemslist0 : items0,
//   itemslist1 : items1
// }
const string PLAYER0("p0");
const string PLAYER1("p1");
const string SUBACC0("s0");
const string SUBACC1("s1");
const string ITEMLIST0("items0");
const string ITEMLIST1("items1");

// Field name of pending array
const PENDING_ARRAY("pending");

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
void makeInfoFromTransaction(BSONElement &pending, TransactionInfo &info);
bool matchRequest(const TransactionInfo &sending, 
		  const TransactionInfo &receiving);
//instantiates a mongo transaction
void executeTransaction(const TransactionInfo &info,
			const BSONObj &subAcc0,
			const BSONObj &subAcc1);
string findTransaction(const TransactionInfo &info, int senderId);
void removeTransaction(const string id);
void logTransaction(const TransactionInfo &info);
//for both users if player to player
void addCompletedTransaction(const TransactionInfo &info, int flag); 
void reverseInfo(TransactionInfo &info);
void cancelTimedOutPending(const string &id, 
			   const string &subaccount);
void cancelAllPending(const string &id, 
		      const string &subaccount);
bool isGlobalAccount(const string &id);
void getAsset(CGI);
void addAccount(const string &id);
void addSubaccount(const string &id, 
		   const string &subaccountId);


// Helper function to get rid of "" of a string
inline void getRidOfQuote(string &str) {
    // get rid of "" surrounding a string
    // e.g. ""hello"" -> "hello"
    if (str[0] == '\"') {
	str = str.substr(1, str.size() - 2);
    }
}

inline bool timedOut() {
    return (time_cnt < TOTAL_TIME);
}

int main(int argc, char **argv) {        
    string EUID = environment.get("EUID");
    string accessToken = environment.get("accessToken");

    if (!Authenticate::authenticateAccessToken(accessToken, EUID)) {
    	return 1;
    }

    handleRequest();

    return 0;
}

int handleRequest() {
    string method = environment.get("method");
    if(method.compare("transaction") == 0) {
        string id0 = environment.get("id0"); //verify id0 == EUID or use EUID directly?
        string id1 = environment.get("id1");
        string subaccountId0 = environment.get("subaccountId0");
	string subaccountId1 = environment.get("subaccountId1");
        string itemsId0 = environment.get("itemsId0");
	string itemsId1 = environment.get("itemsId1");
	
	TransactionInfo info(id0, id1, subaccountId0, 
			     subaccountId1, itemsId0, itemsId1);	
	BSONObj subAcc0, subAcc1;
	
	findSubAcc(info, subAcc0, subAcc1);
	
        if (!checkItemIDs(subAcc0, (fromjson(info.itemsId0)["items"]))) {
            //failed error
	    return -1;
        }
	if (!checkItemIDs(subacc1, (fromjson(info.itemsId1)["items"]))) {
	    return -1;
	}
	
	string subid0 = subAcc0[SUB_ACC_ID_FIELD].toString(false);
	getRidOfQuote(subid0);
	string subid1 = subAcc1[SUB_ACC_ID_FIELD].toString(false);
	getRidOfQuote(subid1);
	
	if (subid0.compare(subid1) < 0) {
	    // Always let the bigger one do check match stuff
	    string accID = subid0;
	    getRidOfQuote(accID);
	    addPendingTransaction(accID, info);
	    return 0;
	}
	
	// The bigger one will check matched pending transaction 
	// several times untill time out
	while (!timedOut()) {
	    // Get the lastest subAcc1
	    subAcc1 = conn.findOne(db_ns, QUERY(SUB_ACC_ID_FIELD << subid1));

	    if (checkMatch(info, subAcc0, subAcc1)) {
		executeTransaction(info, subAcc0, subAcc1);
		return 0;
	    }

	    time_cnt++;
	    this_thread::sleep_for(chrono::seconds(1));
	}
	// Time out
	// Send back error message
	return 0;
    }
    else if (method.compare("addAccount") == 0) {
        //addAccount
    } 
    else if (method.compare("addSubaccount") == 0) {
        //addSubaccount
    } 
    else if (method.comapare("get") == 0) {
        //getAsset
    } 
    else {
        return 2;
    }
}

// Find two subaccounts in the two accounts
bool findSubAcc(const TransactionInfo &info, 
		BSONObj &subAcc0, BSONObj &subAcc1) {
    BSONObj acc = conn.findOne(db_ns, QUERY(ACC_ID_FIELD << info.id0));
    
    if (findSubAccInAcc(acc, info.subaccountId0, subAcc0) == false) {
	return false;
    }
    
    if (info.id0.compare(info.id1) == 0) {
	// Two accounts are the same
	if (info.subaccountId0.compare(info.subaccountId1) == 0) {
	    // If two sub accounts are the same, simply copy it
	    subAcc1 = subAcc0;
	    return true;
	}
	if (findSubAccInAcc(acc, info.subaccountId1, subAcc1) == false) {
	    return false;		
	}
	else {
	    return true;
	}
    }
    
    // Two accounts are different
    acc = conn.findOne(db_ns, QUERY(ACC_ID_FIELD << id1));
    if (findSubAccInAcc(acc, info.subaccountId1, subAcc1) == false) {
	return false;
    }
    
    return true;
}

// Find a subaccount in account
// Here I made the assumption that in an account object
// subaccounts' ids are stored in an array.
// e.g. Account: {"_id":xxxx, "sub_acc_array":[xxx,xxx,xxx,xxx]}.
bool findSubAccInAcc(const BSONObj &acc, const string &index, BSONObj &subAcc) {
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
    
    string subid = id.toString(false);
    getRidOfQuote(subid);
    // use this sub id to find sub account in the db
    subAcc = conn.findOne(db_ns, QUERY(SUB_ACC_ID_FIELD << subid));
    
    return true;
}

// Check if all items are in a subaccount.
// Return flase if a least one is missing.
//
// Assuming item ids in sub account are stored in an array
// e.g. {sub_acc_id : xxx, items : [xxx,xxx,xxx]}
bool checkItemIDs(const BSONObj &subaccount, const BSONElement &items) {
    int i = 0, j;
    BSONElement item0;
    BSONElement item1;
    BSONElement items_array = subaccount[ITEM_ARRAY_FIELD];
    string index0 = to_string(i);
    while ((item0 = items[index0]).ok()) {
	string value0 = item0.toString(false);
	getRidOfQuote(value0);
	j = 0;
	string index1 = to_string(j);
	bool find = false;
	while ((item1 = items_array[index1]).ok()) {
	  string value1 = item1.toString(false);
	  if (value0.compare(value1) == 0) {
	    find = true;
	    break;
	  }
	  j++;
	  index1 = to_string(j);
	}
	if (!find) {
	  return false;
	}
	i++;
	index = to_string(i);
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
    
    // Loop through subacc1's pending transaction
    BSONElement array = subAcc1[PENDING_ARRAY];
    BSONElement pending;
    int i = 0;
    string index = to_string(i);    
    while ((pending = array[index]).ok()) {
	BSONElement p1 = pending[PLAYER1];
	if (p1.ok()) {
	    TransactionInfo info1;
	    makeInfoFromTransaction(pending, info1);
	    if (matchRequest(info, info1)) {
		// Remove pending
		string accID = subAcc0[SUB_ACC_ID_FIELD].toString(false);
		getRidOfQuote(accID);
		removePendingTransaction(accID, info1);
		return true;
	    }
	}
    }        
    
    return false;
}

void makeInfoFromTransaction(BSONElement &pending, TransactionInfo &info) {
    info.id0 = pending[PLAYER0].toString(false);
    getRidOfQuote(info.id0);
    info.id1 = pending[PLAYER1].toString(false);
    getRidOfQuote(info.id1);
    info.subaccountId0 = pending[SUBACC0].toString(false);
    getRidOfQuote(info.subaccountId0);
    info.subaccountId1 = pending[SUBACC1].toString(false);
    getRidOfQuote(info.subaccountId1);
    info.itemsId0 = pending[ITEMLIST0].toString(false);
    getRidOfQuote(info.itemsId0);
    info.itemsId1 = pending[ITEMLIST1].toString(false);    
    getRidOfQuote(info.itemsId1);
}

bool matchRequest(const TransactionInfo &sending, 
		  const TransactionInfo &receiving) {
    int result = 0;
    result += sending.id0.compare(receiving.id1);
    result += sending.id1.compare(receiving.id0);
    result += sending.subaccountId0.compare(receiving.subaccountId1);
    result += sending.subaccountId1.compare(receiving.subaccountId0);
    result += sending.itemsId0.compare(receiving.itemsId1);
    result += sending.itemsId1.compare(receiving.itemsId0);
    
    return (result == 0);
}


// Add pending transaction to a sub account
// Here simply put all information in an json object
// e.g. { p0 : id0,
//        p1 : id1,
//        s0 : subaccountId0,
//        s1 : subaccountId1,
//        items0 : itemsId0,
//        items1 : itemsId1
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
    t.append(SUBACC0, info.subaccountId0);
    t.append(SUBACC1, info.subaccountId1);
    t.append(ITEMLIST0, info.itemsId0);
    t.append(ITEMLIST1, info.itemsId1);
    BSONObjBuilder p;
    p.append(PENDING_ARRAY, t.obj());
    BSONObjBuilder q;
    q.append("$push", p.obj());
    
    conn.update(db_ns, query.obj(), q.obj());
}

void removePendingTransaction(const string &accID, const TransactionInfo &info) {
    BSONObjBuilder query;
    query.append(SUB_ACC_ID_FIELD, accID);

    BSONObjBuilder t;
    t.append(PLAYER0, info.id0);
    t.append(PLAYER1, info.id1);
    t.append(SUBACC0, info.subaccountId0);
    t.append(SUBACC1, info.subaccountId1);
    t.append(ITEMLIST0, info.itemsId0);
    t.append(ITEMLIST1, info.itemsId1);
    BSONObjBuilder p;
    p.append(PENDING_ARRAY, t.obj());
    BSONObjBuilder q;
    q.append("$pull", p.obj());
    
    conn.update(db_ns, query.obj(), q.obj());
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
    if (flag != 0) {
	if (isGlobalAccount(info.id1)) {
	    // Do not log transaction for global account
	    return;
	}
	reverseInfo(info);
    }
    BSONObjBuilder query;
    query.append(ACC_ID_FIELD, info.id0);

    BSONObjBuilder t;
    t.append(PLAYER0, info.id0);
    t.append(PLAYER1, info.id1);
    t.append(SUBACC0, info.subaccountId0);
    t.append(SUBACC1, info.subaccountId1);
    t.append(ITEMLIST0, info.itemsId0);
    t.append(ITEMLIST1, info.itemsId1);
    BSONObjBuilder p;
    p.append(TRANSACTION_HISTORY, t.obj());
    BSONObjBuilder q;
    q.append("$push", p.obj());
    
    conn.update(db_ns, query.obj(), q.obj());
}

// Reverse ids in info 
void reverseInfo(TransactionInfo &info) {
    string id = info.id0;
    string subid = info.subaccountId0;
    string items = info.itemsId0;
    info.id0 = info.id1;
    info.subaccountId0 = info.subaccountId1;
    info.itemsId0 = info.itemsId1;
    info.id1 = id;
    info.subaccountId1 = subid;
    info.itemsId1 = items;
}


void executeTransaction(const TransactionInfo &info,
			const string &subAccId0,
			const string &subAccId1) {
    updateItemArray("$pullAll", subAccId0, info.itemsId0);
    updateItemArray("$pullAll", subAccId1, info.itemsId1);
    updateItemArray("$pushAll", subAccId0, info.itemsId1);
    updateItemArray("$pushAll", subAccId1, info.itemsId0);
    removePendingTransaction(subAccId1, info);
}

// Update items array
//
// Use $pullAll or $pushAll
// 
// db.collection.update( <query>,
//                      { $pullAll: { <arrayField>: [ <value1>, <value2> ... ] } }
//                    )
// db.collection.update( <query>, 
//                      { $pullAll: { <arrayField>: [ <value1>, <value2> ... ] } }
//                   )
//
void updateItemArray(const string method, const string &accID, 
		     const string &itemIDs) {
    BSONObjBuilder query;
    query.append(SUB_ACC_ID_FIELD, accID);
    
    BSONObj items = fromjson(itemIDs);
    
    BSONObjBuilder b;
    b.append(method, items);
        
    conn.update(db_ns, query.obj(), b.obj());        
}
