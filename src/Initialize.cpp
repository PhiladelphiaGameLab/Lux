#include "Initialize.h"

#DEFINE EUID_SIZE 20
#DEFINE ACCESSTOKEN_SIZE 150
#DEFINE DATABASE_NAME "dev.objects"


BSONObj relevantDocuments(BGTInfo bgtInfo){
// find the relevant documents in the mongo database

    // connect to the database
    mongo::DBClientConnection c;
    c.connect("localhost");


    // find the BGT Document and print it out
    BSONObj BGTDoc = c.findOne(DATABASE_NAME, Query("BGT_ID" << bgtInfo.bgt_id));
    printf(BGTDoc.jsonString);

    // for each element in the bucket list
    for (std::list<int bucketID>::iterator bucket = bgtInfo->BucketList.begin(); bucket != bgtInfo->BucketList.end(); bucket++){
            // query the database
            auto_ptr<DBClientCursor> cursor = c.query(DATABASE_NAME, QUERY("bucketID" << bucket) ));
            // print out the elements one by one
            while (cursor->more()){
               printf(cursor->next().jsonString());
            }
    }
}

BSONObj getClientDoc(EUID){

    mongo::DBClientConnection c;
    c.connect("localhost");

    // get the clients document from Mongo
    BSONObj clientDoc = c.findOne(DATABASE_NAME, Query("EUID" << EUID));
    if(clientDoc){
        return clientDoc;
    }

    // create new blank document if one does not exist.
    c.insert(DATABASE_NAME, BSON(GENOID<<"EUID"<<EUID));


    // then:
    clientDoc = c.findOne(DATABASE_NAME, Query("EUID" << EUID));
    if(clientDoc){
        return clientDoc;
    }
    string err = c.getLastError();
    return NULL;
}


int main(int argc, char *argv[]){
    std::string GET  = CGI::start();
    std::string EUID = CGI::get(GET, "EUID", EUID_SIZE);
    std::string EUID = CGI::get(GET, "accessToken", ACCESSTOKEN_SIZE);

    if (GET != ""){
        // find the EUID & access Token
        std::string EUID = CGI::get(GET, "EUID", EUID_SIZE);
        std::string EUID = CGI::get(GET, "accessToken", ACCESSTOKEN_SIZE);

        // validate that they were found before moving forward
        if(!EUID != "" && !accessToken != ""){
            // Authenticate access token
            if(Authenticate::authenticateAccessToken(accessToken, EUID)){
                // query mongo for user info
                BSONObj clientDocument = getClientDoc(EUID);

                // query FindBGT
                BGTInfo bgtInfo= findBGT::find(clientDocument);

                BSONObj rDocs = relevantDocuments(bgtInfo);
            }
        }
    }

  return EXIT_SUCCESS;
}
