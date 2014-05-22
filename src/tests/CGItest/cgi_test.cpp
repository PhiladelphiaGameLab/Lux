#include "../../classes/instanceable/CGI.h"
#include <iostream>
#include <cstdlib>
#include <string>


int main(int argc, char *argv[]){
    
    putenv("REQUEST_METHOD=GET");
    putenv("QUERY_STRING=?test=fun%7B&name=Jake+Ailor&team=true&EUID=12343");
    putenv("CONTENT_LENGTH=49");

    std::cout << CGI::getEnvStr("QUERY_STRING") << std::endl;

    CGI environment;

    // Make json object one by one
    environment.clearJSON();
    if (environment.addJSON("test", environment.get("test")) < 0) {
	environment.error("User requested undefined field.", 0);
    }
    if (environment.addJSON("name", environment.get("name")) < 0) {
	environment.error("User requested undefined field.", 0);
    }
    if (environment.addJSON("team", environment.get("team")) < 0) {
	environment.error("User requested undefined field.", 0);
    }
    if (environment.addJSON("EUID", environment.get("EUID")) < 0) {
	environment.error("User requested undefined field.", 0);
    }
    environment.printJSON();

    std::list<std::string> arr;
    arr.push_back("test");
    arr.push_back("name");
    arr.push_back("team");
    arr.push_back("EUID");
    // Make json object using list of field names
    environment.makeJSON(arr);
    environment.printJSON();

    // Get json object in string format
    std::string tmp = environment.getJSON();
    std::cout << tmp << std::endl;

    /*
    std::cout << "test (fun): " << enviornment.get("test") << std::endl;
    std::cout << "name (jake): " << enviornment.get("name") << std::endl;
    std::cout << "team (true): " << enviornment.get("team") << std::endl;
    std::cout << "EUID (12343): " << enviornment.get("EUID") << std::endl;
    */
    return EXIT_SUCCESS;
}
