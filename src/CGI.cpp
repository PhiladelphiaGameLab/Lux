#include "CGI.h"

static std::string CGI::getEnvStr(std::string const& key){
    char const* val = getenv(key.c_str());
    return val == NULL ? std::string() : std::string(val);
}

static std::string CGI::get(std::string str, std::string parameter, size_t size){

    // Sanatize input

    parameter += "=";
    std::size_t foundStart = str.find(parameter);
    std::string found;
    if (foundStart != std::string::npos){
        found = std::string str2 = str.substr (foundStart,size));
    }
    return found;
}

static std::string CGI::init(){
    // send the content type first
    printf("Content-Type: text/html\n\n");

    return getEnvStr("QUERY_STRING");
}
