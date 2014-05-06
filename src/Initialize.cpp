#include "Initialize.h"

#DEFINE EUID_SIZE 20
#DEFINE ACCESSTOKEN_SIZE 150
#DEFINE SERVER_SECRET "LUXisAwesome!"
#DEFINE DATABASE_NAME "dev.objects"


std::string getEnvStr(std::string const& key){
    char const* val = getenv(key.c_str());
    return val == NULL ? std::string() : std::string(val);
}

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
}


int main(int argc, char *argv[]){
    // send the content type first
    printf("Content-Type: text/html\n\n");
    std::string GET = getEnvStr("QUERY_STRING");
    std::string EUIDpos ("EUID=");
    std::string accessTokenpos ("accessToken=");
    if (GET != ""){
        // find the EUID
        std::size_t foundEUID = str.find(EUIDpos);
        if (foundEUID != std::string::npos){
            std::string EUID = std::string str2 = str.substr (foundEUID,EUID_SIZE);
        }
        // find the Access Token
        std::size_t foundAccessToken = str.find(accessTokenpos);
        if (foundAccessToken != std::string::npos){
            std::string accessToken = std::string str2 = str.substr (foundAccessToken,ACCESSTOKEN_SIZE);
        }
        // validate that they were found before moving forward
        if(!EUID.Equals(null) && !accessToken.Equals(null)){
            // Authenticate access token
            if(authenticate::AuthenticateAccessToken(accessToken, EUID, SERVER_SECRET)){
                // query mongo for user info
                BSONObj clientDocument = getClientDoc(EUID);

                // query FindBGT
                BGTInfo bgtInfo= findBGT::find(clientDocument);

                printf("{");
                // query mongo for relevant documents including BGT document (with socket)
                printf("\tobjects:\n");
                BSONObj rDocs = relevantDocuments(bgtInfo);
                printf("}");

            }
        }
    }

  return EXIT_SUCCESS;
}
