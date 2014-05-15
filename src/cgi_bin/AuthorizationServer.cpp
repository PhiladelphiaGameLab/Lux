#include "AuthorizationServer.h"

int main(int argc, char *argv[]){

    CGI environment; // create instance of CGI Class

    string JWT = environment.get("JWT"); // get JWT
    string APIKey = environment.get("APIKey"); // Get client APIKey

    string uniqueID = Authenticate::authenticateJWT(JWT, APIKey); // create a unique ID with the Authenticate class

    if(uniqueID != ""){
		//std::vector<mongo::BSONObj> bulk_data;
        
		try{
		mongo::DBClientConnection c;
		 c.connect("localhost");
		 // Handle new Users
		//search inside DB if it does match any existing keys, then create a new EUID
		//TODO
		
		//I know this is a new user
		 string newEUID = Authenticate::createNewEUID(JWT, APIKey);
		 mongo::BSONObj newUser = BSON("Type"<<"Auth"<<newEUID);   // need to be modified
		 c.insert("Type"<<"Auth"<<uniqueID, newUser);
            // save all the other data passed in from the Query String
        
		  // handle false
		 
        // query Key-Value store   access MongoDB
		    
			// find the relevant documents in the mongo database
			// find the EUID Document and print it out
			BSONObj EUIDDoc = c.findOne(DATABASE_NAME, Query("Type"<<"Auth"<<uniqueID));    //how to search in MongoDB
			//take out the EUID from the BSONObj
			std::string EUID = EUIDDoc["_id"].toString();



    }catch{
		
        // then:
        string err = c.getLastError();
        return NULL;
    }
}

        // get back EUID
        string EUID = ; // access via uniqueID from JWT

        // write to database with new access token

        string accessToken = Authenticate::createAccessToken(EUID);

        cout << '{\n"EUID":"' << EUID << '",\n"AccessToken":"'<< accessToken << '"}' << endl; // return the value to the user

    }else{
		   
            // Handle errors
            // Handle incorrect JWT/APIKey pairs or whatever
		    CGI::error("not a token", 9);    //unique integer indicating error
    }

    return EXIT_SUCCESS;
}
