#include "AuthorizationServer.h"
using namespace std;
int main(int argc, char *argv[]){

    CGI environment; // create instance of CGI Class

    string JWT = environment.get("JWT"); // get JWT
    string APIKey = environment.get("APIKey"); // Get client APIKey
	string RefreshToken = environment.get("RefreshToken");   //Get Refresh Token

    string uniqueID = Authenticate::authenticateJWT(JWT, APIKey); // create a unique ID with the Authenticate class

    if(uniqueID != ""){
		//vector<mongo::BSONObj> bulk_data;
        
		try{
		mongo::DBClientConnection c;
		 c.connect("localhost");
		 // Handle new Users
		//search inside DB if it doesn't match any existing keys, then create a new EUID
		BSONObj EUIDDoc = c.findOne(DATABASE_NAME, Query("_id"<<uniqueID));
		if(EUIDDoc == NULL)
		{
		//I know this is a new user
		 string newEUID = Authenticate::createNewEUID(uniqueID);
		 mongo::BSONObj newUser = BSON("_id"<<uniqueID<<"EUID"<<newEUID<<"APIKey"<<APIKey<<"RefreshToken"<<RefreshToken);   // save all the other data passed in from the Query String
		 c.insert(DATABASE_NAME, newUser);     
		} 
		else {// handle false
		 
        // query Key-Value store   access MongoDB
		    
			// find the relevant documents in the mongo database
			// find the EUID Document and print it out
			//take out the EUID from the BSONObj
			string EUID = EUIDDoc["EUID"].toString();
		}


    }catch{
		
        // then:
        string err = c.getLastError();
        return NULL;
    }


        // get back EUID
        //string EUID = ; // access via uniqueID from JWT

        // write to database with new access token
        string accessToken = Authenticate::createAccessToken(EUID);
        cout << '{\n"EUID":"' << EUID << '",\n"AccessToken":"'<< accessToken << '"}' << endl; // return the value to the user

    }else{
		   
            // Handle errors
            // Handle incorrect JWT/APIKey pairs or whatever
		    CGI::error("Invalid UniqueID!", 5);    //unique integer indicating error
    }

    return EXIT_SUCCESS;
}
