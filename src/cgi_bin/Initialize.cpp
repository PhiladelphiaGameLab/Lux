#include "Initialize.h"

void Initialize::relevantDocuments(BGTInfo bgtInfo){

    try{
        // find the relevant documents in the mongo database

        // connect to the database
        mongo::DBClientConnection c;
        c.connect("localhost");


        // find the BGT Document and print it out
        BSONObj BGTDoc = c.findOne(DATABASE_NAME, Query("Type" << "BGT" << "BGT_ID" << bgtInfo.bgt_id));
        std::cout << BGTDoc.jsonString << std::endl;

        // for each element in the bucket list
        for (std::list<int>::iterator bucket = bgtInfo.BucketList.begin(); bucket != bgtInfo.BucketList.end(); bucket++){
                // query the database
                auto_ptr<DBClientCursor> cursor = c.query(DATABASE_NAME, QUERY("BGT_ID" << bgtInfo.bgt_id << "bucketID" << bucket) ));
                // print out the elements one by one
                while (cursor->more()){
                   std::cout << cursor->next().jsonString() << std::endl;
                }
        }
    }catch{
        // then:
        string err = c.getLastError();
        return NULL;
    }
}

BSONObj Initialize::getClientDoc(EUID){

    try{
        mongo::DBClientConnection c;
        c.connect("localhost");

        // get the clients document from Mongo
        BSONObj clientDoc = c.findOne(DATABASE_NAME, Query( _id << EUID << "EUID" << EUID));
        if(clientDoc){
            return clientDoc;
        }else{
            // create new blank document if one does not exist.
            c.insert(DATABASE_NAME, BSON(_id << EUID << "EUID" <<EUID));
            return getClientDoc(EUID);
        }
    }catch{
        // then:
        string err = c.getLastError();
        return NULL;
    }
}


int main(int argc, char *argv[]){

    // get parameters from the HTTP Request
    CGI environment
    std::string EUID = environment.get("EUID");
    std::string accessToken = environment.get("accessToken");

    // validate that they were found before moving forward
    if(!EUID != "" && !accessToken != ""){
        // Authenticate access token
        if(Authenticate::authenticateAccessToken(accessToken, EUID)){
            // query mongo for user info
            BSONObj clientDocument = Initialize::getClientDoc(EUID);
            if(clientDocument != NULL){
                    // query FindBGT
                    s_BGTInfo bgtInfo = findBGT::find(clientDocument);

                    Initialize::relevantDocuments(bgtInfo);
            }else{
                CGI::error("Client Does Not Exist. Please try Rehashing", 1);
            }
        }else{
            CGI::error("Invalid Access Token. Please try Rehashing", 2);
        }
    }else{
        CGI::error("Missing Access Token or End-User ID", 3)
    }

  return EXIT_SUCCESS;
}
