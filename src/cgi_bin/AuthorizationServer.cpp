#include "AuthorizationServer.h"
using namespace std;

void run(){
		mongo::DBClientConnection c;
		 c.connect("localhost");
}

int main(int argc, char *argv[]){

    CGI environment; // create instance of CGI Class

    string JWT = environment.get("JWT"); // get JWT
    string APIKey = environment.get("APIKey"); // Get client APIKey
	string RefreshToken = environment.get("RefreshToken");   //Get Refresh Token

    string uniqueID = Authenticate::authenticateJWT(JWT, APIKey); // create a unique ID with the Authenticate class

    if(uniqueID != ""){
		try{
         run();
		 std::cout << "connected ok" << std::endl;  //successfully connect
		 // Handle new Users
		//search inside DB if it doesn't match any existing keys, then create a new EUID
		BSONObj EUIDDoc = c.findOne(DATABASE_NAME, Query("_id"<<uniqueID));
		string EUID = "";
		if(EUIDDoc == NULL)
		{
		//I know this is a new user
		 string newEUID = Authenticate::createNewEUID(uniqueID);
		 EUID = newEUID;
		 mongo::BSONObj newUser = BSON("_id"<<uniqueID<<"EUID"<<newEUID<<"APIKey"<<APIKey<<"RefreshToken"<<RefreshToken);   // save all the other data passed in from the Query String
		 c.insert(DATABASE_NAME, newUser);     
		} 
		else {
			// handle old Users
			//take out the EUID from the BSONObj
			EUID = EUIDDoc["EUID"].toString();
		}

    }catch( const mongo::DBException &e ) {
        std::cout << "caught " << e.what() << std::endl;
    }

        string accessToken = Authenticate::createAccessToken(EUID);
		// write to database with new access token, corresponding to the correct unique ID
		BSONObj AccessDoc = c.findOne(DATABASE_NAME, Query("_id"<<uniqueID));
		AccessDoc.put("AccessToken", accessToken);

        cout << '{\n"EUID":"' << EUID << '",\n"AccessToken":"'<< accessToken << '"}' << endl; // return the value to the user

    }else{
		   
            // Handle errors
            // Handle incorrect JWT/APIKey pairs or whatever
		    CGI::error("Invalid UniqueID!", 5);    //unique integer indicating error
    }

    return EXIT_SUCCESS;
}
