#include "../classes/instanceable/CGI.h"
#include <iostream>
#include <cstdlib>
#include <string>


int main(int argc, char *argv[]){

    putenv("REQUEST_METHOD=GET");
    putenv("QUERY_STRING=?test=fun&name=jake&team=true&EUID=12343");
    putenv("CONTENT_LENGTH=41");


    CGI enviornment;
    std::cout << enviornment.get("test") << std::endl;
    std::cout << enviornment.get("name") << std::endl;
    std::cout << enviornment.get("team") << std::endl;
    std::cout << enviornment.get("EUID") << std::endl;

    putenv("REQUEST_METHOD=POST");
    putenv("QUERY_STRING=?test=fun2&name=jake2&team=true2&EUID=123432");
    putenv("CONTENT_LENGTH=45");

    CGI enviornment2;
    std::cout << enviornment2.get("test") << std::endl;
    std::cout << enviornment2.get("name") << std::endl;
    std::cout << enviornment2.get("team") << std::endl;
    std::cout << enviornment2.get("EUID") << std::endl;


return EXIT_SUCCESS;
}
