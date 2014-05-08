#include "AuthorizationServer.h"

int main(int argc, char *argv[]){

    CGI enviorment; // create instance of CGI Class

    std::string JWT = enviorment.get("JWT"); // get JWT
    std::string APIKey = enviorment.get("APIKey"); // Get client APIKey

    std::string uniqueID = Authenticate::authenticateJWT(JWT, APIKey); // create a unique ID with the Authenticate class

    if(uniqueID != ""){
        // query Key-Value store
        // get back EUID
        std::string EUID = ;

        std::string accessToken = Authenticate::createAccessToken(EUID);
        printf('{\n"EUID":"%s",\n"AccessToken":"%s"}', EUID, accessToken);

    return EXIT_SUCCESS;
}
