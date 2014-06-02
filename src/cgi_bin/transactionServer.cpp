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

using namespace std;
using namespace mongo;

struct TransactionInfo
{
    std::string id0; 
    std::string id1;
    std::string subaccountId0;
    std::string subaccountId1;
    std::string itemsId0;
    std::string itemsId1;
    TransactionInfo(std::string i0, std::string i1,
                    std::string si0, std::string si1,
                    std::string iti0, std::string iti1):
        id0(i0), id1(i1), subaccountId0(si0), subaccountId1(si1),
        itemsId0(iti0), itemsId1(iti1) {}
};

bool checkItemIDs(const std::string &id, 
                const std::string &subaccount,
                const mongo::BSONElement &items);
bool checkItem(const std::string &id,
                const std::string &subaccount, 
                const std::string &item);
void addPendingTransaction(const transactionInfo &info);
bool matchRequest(const mongo::BSONElement &sending, 
                const mongo::BSONElement &receiving);
void executeTransaction(const transactionInfo &info); //instantiates a mongo transaction
std::string findTransaction(const transactionInfo &info, int senderId);
void removeTransaction(const std::string id);
void addCompletedTransaction(const transactionInfo &info); //for both users if player to player
void cancelTimedOutPending(const std::string &id, 
                    const std::string &subaccount);
void cancelAllPending(const std::string &id, 
                    const std::string &subaccount);
bool isGlobalAccount(const std::string &id);
void getAsset(CGI);
void addAccount(const std::string &id);
void addSubaccount(const std::string &id, 
                    const std::string &subaccountId);

int main() {

    // get parameters from the HTTP Request
	CGI environment;
    MongoWrapper conn("localhost");
    
	std::string EUID = environment.get("EUID");
    std::string accessToken = environment.get("accessToken");

    if(!Authenticate::authenticateAccessToken(accessToken, EUID)) {
    	return 1;
    }

    std::string method = environment.get("method");
    
    if(method.compare("transaction") == 0) {
        std::string id0 = environment.get("id0"); //verify id0 == EUID or use EUID directly?
        std::string id1 = environment.get("id1");
        std::string subaccountId0 = environment.get("subaccountId0");
        std::string itemsId0 = environment.get("itemsId0");
        if(!checkItemIDs(&id0, &subaccountId0, &itemsId0)) {
            //failed error
        }
        if(id0.compare(id1) == 0 || isGlobalAccount(id1)) { //unmatched transaction
            std::string subaccountId1 = environment.get("subaccountId1");
            std::string itemsId1 = environment.get("itemsId1");
            if(!checkItemIDs(&id1, &subaccountId1, &itemsId1)) {
                //failed error
            }
            TransactionInfo info(id0, id1, subaccountId0, 
                subaccountId1, itemsId0, itemsId1);
            executeTransaction(&info);
            //reply successful
        } else { //matched transaction
            //checks for match
            executeTransaction(&info);
        }

    } else if(method.compare("addAccount") == 0) {
        //addAccount
    } else if(method.compare("addSubaccount") == 0) {
        //addSubaccount
    } else if(method.comapare("get") == 0) {
        //getAsset
    } else {
        return 2;
    }

    return 0;
}

bool checkItemIDs(const std::string &id, const std::string &subaccount, 
                    const mongo::BSONElement &items) {
    if(items.length == 0) {
        return true;
    }
    for(int i = 0; i < items.length; i++) {
        if(!checkItem(id, subaccount, items[i])) {
            return false;
        }
    }
    return true;
}

bool checkItem(const std::string &id, const std::string &subaccount, 
                const std::string &item) {
    //return search subaccount array for item != null
}