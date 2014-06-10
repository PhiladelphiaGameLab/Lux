#include "AuthorizationServer.h"
using namespace std;
using namespace mongo;


int main(int argc, char *argv[]){

    CGI environment; // create instance of CGI Class

    string JWT = environment.get("JWT"); // get JWT
    string APIKey = environment.get("APIKey"); // Get client APIKey
	string RefreshToken = environment.get("RefreshToken");   //Get Refresh Token

    string uniqueID = Authenticate::authenticateJWT(JWT, APIKey); // create a unique ID with the Authenticate class
	string EUID = "";
    if(uniqueID != ""){
	try{
        	 mongo::DBClientConnection c;
                 c.connect("localhost");

		 std::cout << "connected ok" << std::endl;  //successfully connect
		 // Handle new Users
		//search inside DB if it doesn't match any existing keys, then create a new EUID
		BSONObj EUIDDoc = c.findOne(DATABASE_NAME, QUERY("_id"<<uniqueID));
		if(EUIDDoc.isEmpty()){
		//I know this is a new user
			string newEUID = Authenticate::createNewEUID(uniqueID);
			EUID = newEUID;
			mongo::BSONObj newUser = BSON("_id"<<uniqueID<<"EUID"<<newEUID<<"APIKey"<<APIKey<<"RefreshToken"<<RefreshToken);   // save all other data passed in from the Query String
			c.insert(DATABASE_NAME, newUser);     
		}else {
			// handle old Users
			//take out the EUID from the BSONObj
			EUID = EUIDDoc["EUID"].toString();
		}
        	string accessToken = Authenticate::createAccessToken(EUID);
                // write to database with new access token, corresponding to the correct unique ID
                //BSONObj AccessDoc = c.findOne(DATABASE_NAME, QUERY("_id"<<uniqueID));
                //AccessDoc.put("AccessToken", accessToken);
      
                cout << "{\n\"EUID\":\"" << EUID << "\",\n\"AccessToken\":\""<< accessToken << "\"}" << endl; // return the value to the user
                
	}catch( const mongo::DBException &e ) {
        	std::cout << "caught " << e.what() << std::endl;
	}

    }else{
		   
            // Handle errors
            // Handle incorrect JWT/APIKey pairs or whatever
		    environment.error("Invalid UniqueID!", 5);    //unique integer indicating error
    }

    return EXIT_SUCCESS;
}
