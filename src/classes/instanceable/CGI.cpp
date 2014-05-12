#include "CGI.h"


/*
 - Parses both GET and POST form data transparently
 - Provides methods for saving and restoring CGI environments to aid
   in application debugging
 - Supports HTTP file upload
*/


CGI::CGI(){
    cout << "Content-Type: text/html\n\n" << endl;
    get = getEnvStr("QUERY_STRING");
    // handle POST and PUT as well

    if(getEnvStr("REQUEST_METHOD").compare("GET") == 0){

    }else if(getEnvStr("REQUEST_METHOD").compare("POST") == 0){

    }else if(getEnvStr("REQUEST_METHOD").compare("PUT") == 0){

    }
}

static std::string CGI::getEnvStr(std::string const& key){
    char const* val = getenv(key.c_str());
    return val == NULL ? std::string() : std::string(val);
}

std::string CGI::get(std::string parameter, size_t size){

    // Sanatize input

    parameter += "=";
    std::size_t start = get.find(parameter);
    std::size_t end = get.find("&", start);
    std::string found;
    if (start != std::string::npos){
        if(end != std::string::npos){
            found = get.substr(start,end-start);
        }else{
            found = get.substr(start,strlen(get)-start);
        }
    }
    return found;
}




