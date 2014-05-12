#include "AuthorizationServer.h"

int main(int argc, char *argv[]){

    CGI environment; // create instance of CGI Class

    string JWT = environment.get("JWT"); // get JWT
    string APIKey = environment.get("APIKey"); // Get client APIKey

    string uniqueID = Authenticate::authenticateJWT(JWT, APIKey); // create a unique ID with the Authenticate class

    if(uniqueID != ""){

        // Handle new Users
            // create a new EUID
            // save all the other data passed in from the Query String
        // handle false

        // query Key-Value store
        // get back EUID
        string EUID = ; // access via uniqueID from JWT

        // write to database with new access token

        string accessToken = Authenticate::createAccessToken(EUID);

        cout << '{\n"EUID":"' << EUID << '",\n"AccessToken":"'<< accessToken << '"}' << endl; // return the value to the user

    }else{
            // Handle errors
            // Handle incorrect JWT/APIKey pairs or whatever
    }

    return EXIT_SUCCESS;
}
