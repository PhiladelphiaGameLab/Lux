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

using namespace std;
using namespace mongo;

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

// connection to mongo db
MongoWrapper conn("localhost"); 
// the namespace of collection
const string db_ns("db.transaction"); 
// the field name for account id
const string ACC_ID_FIELD("_id"); 
// field name for subaccount, default is equal to account id
const string SUB_ACC_ID_FIELD(ACC_ID_FIELD);
const string SUB_ACC_ARRAY_FILED("sub_acc_array");

int handleRequest();
bool findSubAcc(const TransactionInfo &info, BSONObj &subAcc0, BSONObj &subAcc1);
bool findSubAccInAcc(const BSONObj &acc, const string &index, BSONObj &subAcc);
bool checkItemIDs(const BSONObj &subaccount, const BSONElement &items);
bool checkMatch(const TransactionInfo &info, BSONObj &subAcc0, BSONObj &subAcc1);
void addPendingTransaction(const string &accID, const transactionInfo &info);
bool matchRequest(const BSONElement &sending, 
                const BSONElement &receiving);
void executeTransaction(const transactionInfo &info); //instantiates a mongo transaction
string findTransaction(const transactionInfo &info, int senderId);
void removeTransaction(const string id);
void addCompletedTransaction(const transactionInfo &info); //for both users if player to player
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

int main(int argc, char **argv) {    
    // get parameters from the HTTP Request
    CGI environment;    
    
    string EUID = environment.get("EUID");
    string accessToken = environment.get("accessToken");

    if(!Authenticate::authenticateAccessToken(accessToken, EUID)) {
    	return 1;
    }

    // I think transaction server should be a program that runs as 
    // one of the game servers. Now it seems it was designed as a
    // CGI program which woule be ivoked by game server many times.
    
    // 

    While (1) {
	// waiting for request
	// start a new thread
	// and call handleRequest()
	handleRequest();
     }
    //
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
	
        if (!checkItemIDs(subAcc0, fromjson(info.itemsId0))) {
            //failed error
	    return -1;
        }
	if (!checkItemIDs(subacc1, fromjson(info.itemsId1))) {
	    return -1;
	}
	
	if (checkMatch(info, subAcc0, subAcc1)) {	    
	    executeTransaction(info);
	}
	else {
	    string accID = subAcc0[SUB_ACC_ID_FIELD].toString(false);
	    getRidOfQuote(accID);
	    addPendingTransaction(accID, info);
	}
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
bool checkItemIDs(const BSONObj &subaccount, const BSONElement &items) {
    int i = 0;
    BSONElement item;
    string index = to_string(i);
    while ((item = items[index]).ok()) {
	string key = item.toString(false);
	getRidOfQuote(key);
	if (subaccount[key].ok() == 0) {
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

    // Transaction between two different players
    // Check "pending" filed in two sub accounts' BSON Objects
    //
    // Need to discuss with Brad, in what form do we want to 
    // store the pending transaction info.
    //
    // if (match)
    //   return true;
    //
    return false;
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
    t.append("p0", info.id0);
    t.append("p1", info.id1);
    t.append("s0", info.subaccountId0);
    t.append("s1", info.subaccountId1);
    t.append("items0", info.itemsId0);
    t.append("items1", info.itemsId1);
    BSONObjBuilder p;
    p.append("pending", t.obj());
    BSONObjBuilder q;
    q.append("$push", p.obj());
    
    conn.update(db_ns, query.obj(), q.obj());
}

