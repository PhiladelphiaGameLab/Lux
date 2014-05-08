#include "CGI.h"

CGI::CGI(){
    printf("Content-Type: text/html\n\n");
    get = getEnvStr("QUERY_STRING");
    // handle POST and PUT as well
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
    std::string foud;
    if (start != std::string::npos){
        if(end != std::string::npos){
            found = get.substr(start,end-start);
        }else{
            found = get.substr(start,strlen(get)-start);
        }
    }
    return found;
}

