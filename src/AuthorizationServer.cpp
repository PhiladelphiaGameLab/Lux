#include "AuthorizationServer.h"
#include "Authenticate.h"
#include "CGI.h"
int main(int argc, char *argv[]){

    std::string GET  = CGI::init();
    std::string JWT = CGI::get(GET, "JWT", EUID_SIZE);
    std::string APIKey = CGI::get(GET, "APIKey", ACCESSTOKEN_SIZE);

    std::string uniqueID = Authenticate::authenticateJWT(JWT, APIKey);
    if(uniqueID != ""){
        // query Key-Value store
        // get back EUID
        std::string EUID = ;

        std::string accessToken = Authenticate::createAccessToken(EUID);
        printf('{\n"EUID":"%s",\n"AccessToken":"%s"}', EUID, accessToken);

    return EXIT_SUCCESS;
}
