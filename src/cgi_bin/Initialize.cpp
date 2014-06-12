#include "Initialize.h"
using namespace mongo;
using namespace std;

void Initialize::relevantDocuments(s_BGTInfo bgtInfo){

    CGI environment;
    try{
        // find the relevant documents in the mongo database

        // connect to the database
        DBClientConnection c;
        c.connect("localhost");


        // find the BGT Document and print it out
        BSONObj BGTDoc = c.findOne(DATABASE_NAME, QUERY("Type" << "BGT" << "BGT_ID" << bgtInfo.bgt_id));
        environment.addJSON(BGTDoc.toString(Strict,1));
        
	for (vector<int>::iterator bucket = bgtInfo.buckets.begin(); bucket != bgtInfo.buckets.end(); bucket++){
            // query the database
            auto_ptr<DBClientCursor> cursor = c.query(DATABASE_NAME, QUERY("bucketID" << (*bucket)));
            // iterate elements from the buckets
            while (cursor->more()){
                // strip sender access token & such
                // send both client and message to the socket Class
        	environment.addJSON(cursor->next().toString(Strict, 1));
	    }
        }
	environment.printJSON();
    }catch( const mongo::DBException &e ){
        // then:
        environment.error("Internal Server Error", 500);
    }
}

BSONObj Initialize::getClientDoc(string EUID){

    CGI environment;
    try{
        DBClientConnection c;
        c.connect("localhost");

        // get the# clients document from Mongo
        BSONObj clientDoc = c.findOne(DATABASE_NAME, QUERY("EUID" << EUID));
        if(clientDoc.isEmpty()){
            // create new blank document if one does not exist.

	BSONObj loc = BSON( "x" << 50 << "y" << 50);
	BSONObj obj = BSON("location"<< loc << "radius" << 2 << "EU_DOC" << true);
	BSONObj sender = BSON("accessToken" << "#324234asdkf" << "EUID" << EUID);
	c.insert(DATABASE_NAME, BSON(GENOID << "object" << obj << "sender" << sender));

	    return getClientDoc(EUID);
        }else{
            return clientDoc;
        }
    }catch( const mongo::DBException &e ){
        // then:
        environment.error("Internal Server Error", 500);
        return BSONObj();
    }
}


int main(){

    // get parameters from the HTTP Request
    CGI environment;
    string EUID = environment.get("EUID");
    string accessToken = environment.get("accessToken");

    // validate that they were found before moving forward
    if(EUID.compare("") != 0 && accessToken.compare("") != 0){
        // Authenticate access token
        if(Authenticate::authenticateAccessToken(accessToken, EUID)){
            // query mongo for user info
            BSONObj clientDocument = Initialize::getClientDoc(EUID);
            if(!clientDocument.isEmpty()){
                    // query FindBGT
                    s_BGTInfo bgtInfo = FindBGT::find(clientDocument);
                    Initialize::relevantDocuments(bgtInfo);
            }else{
                environment.error("Client Does Not Exist. Please try Rehashing", 1);
            }
        }else{
            environment.error("Invalid Access Token. Please try Rehashing", 2);
        }
    }else{
        environment.error("Missing Access Token or End-User ID", 3);
    }

  return EXIT_SUCCESS;
}
