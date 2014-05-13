#include "../classes/instanceable/CGI.h"
#include <iostream>
#include <cstdlib>
#include <string>


int main(int argc, char *argv[]){

    putenv("REQUEST_METHOD=GET");
    putenv("QUERY_STRING=?test=fun%7B&name=Jake+Ailor&team=true&EUID=12343");
    putenv("CONTENT_LENGTH=41");

    std::cout << CGI::getEnvStr("QUERY_STRING") << std::endl;

    CGI enviornment;
    std::cout << "test (fun): " << enviornment.get("test") << std::endl;
    std::cout << "name (jake): " << enviornment.get("name") << std::endl;
    std::cout << "team (true): " << enviornment.get("team") << std::endl;
    std::cout << "EUID (12343): " << enviornment.get("EUID") << std::endl;

    putenv("REQUEST_METHOD=POST");
    putenv("QUERY_STRING=?test=fun2&name=jake2&team=true2&EUID=123432");
    putenv("CONTENT_LENGTH=45");

    std::cout << CGI::getEnvStr("QUERY_STRING") << std::endl;

    CGI enviornment2;
    std::cout << "test (fun2): " << enviornment2.get("test") << std::endl;
    std::cout << "name (jake2): " << enviornment2.get("name") << std::endl;
    std::cout << "team (true2): " << enviornment2.get("team") << std::endl;
    std::cout << "EUID (123432): " << enviornment2.get("EUID") << std::endl;


return EXIT_SUCCESS;
}
