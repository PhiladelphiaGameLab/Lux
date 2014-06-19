#include "CGI.h"

// done
std::string CGI::decode_string(const std::string &str) {
   const char *digits = "0123456789ABCDEF";
   std::string result = "";
   int length = str.length();
   int pos;
   for (pos = 0; pos < length; pos++) {
       if (str[pos] == '+') {
	   // Handle white space
	   result.push_back(' ');
       }
       else if (str[pos] == '%') {
	   // Handle hex characters
	   char ch1 = toupper(str[++pos]);
	   char ch2 = toupper(str[++pos]);
	   result.push_back(16 * (strchr(digits, ch1) - strchr(digits,'0')) +
			     strchr(digits, ch2) - strchr(digits,'0'));
       }
       else {
	   //Handle remaining characters
	   result.push_back(str[pos]);
       }
   }
   return result;
}

CGI::CGI() : argCnt(0){
    // done
    // Initialize private variables
    for (int pos = 0; pos < MAX_ARGS; pos++){
        names[pos] = values[pos] = "";
    }
    // Read environment variables
    std::string request_method = CGI::getEnvStr("REQUEST_METHOD");
    std::string query_string = CGI::getEnvStr("QUERY_STRING");
    std::string content_length = CGI::getEnvStr("CONTENT_LENGTH");

    int query_length = 0;

    // Check request_method variable
    if (request_method.compare("")  == 0)
	return;

    if (request_method.compare("GET") == 0) {
	// Handle GET requests
	if (query_string != "") {
	    if (content_length == "") {
		query_length = query_string.length();
	    }
	    else {
		query_length = atoi(content_length.c_str());
	    }
	}
    }
    else if (request_method.compare("POST") == 0) {
	// Handle POST requests
	if (content_length.compare("") != 0) {
	    int post_length;
	    post_length = atoi(content_length.c_str());
	    // Need to figure out how the content is organized in POST method
	    return;
	}
    }
    /*
    int post_length;
    char *post_string;
    // Handle POST requests

    if (content_length != ""){
	post_length = atoi(content_length.c_str());
	post_string = (char *)malloc(post_length);
	if (post_string != "") {
	    for (int pos = 0; pos < post_length; pos++){
		post_string[pos] = fgetc(stdin);
	    }
	    JSONin = mongo::fromjson(post_string, post_length);
	}
	free(post_string);
    }
    */

    // Separate query_string into arguments
    int start_name, end_name, start_value, end_value = 0;
    while (end_value < query_length){
	// Find argument name
	
	start_name = end_name = end_value + 1;
	while ((end_name < query_length) && query_string[end_name] != '=') {
	    end_name++;
	}

	// Copy and decode name string
	std::string xd = query_string.substr(start_name, end_name - start_name);
	names[argCnt] = decode_string(xd);

	// Find argument value
	start_value = end_value = end_name + 1;
	while ((end_value<query_length) && query_string[end_value] != '&'){
	    end_value++;
	}

	// Copy and decode value string
	std::string md = query_string.substr(start_value, end_value - start_value);
	values[argCnt] = decode_string(md);

	argCnt++;
	std::cout << "Content-type: text/html\n\n" << std::endl;

    }   
}

// done
mongo::BSONElement CGI::get(const std::string &name, bool bson) {
    if (JSONin.hasElement(name.c_str())) {
	return JSONin[name];
    }
    return mongo::BSONElement();
}

std::string CGI::get(const std::string &name) {
    int i;
    for (i=0; i < argCnt; i++) {
	if (name.compare(names[i]) == 0) {
	    std::string ret = "";
	    ret.push_back('\"');
	    ret += values[i];
	    ret.push_back('\"');
	    return ret;
	}
    }
    
    if(JSONin.hasElement(name.c_str())) {
        return JSONin[name].toString();
    }
    return "";
}

// done
std::string CGI::getName(int index) {
    // Lookup argument by location
    if ((index >= 0) && (index < argCnt)) {
	return names[index];
    }
    return "";
}

// done
std::string CGI::getValue(int index) {
    // Lookup argument by location
    if ((index >= 0) && (index < argCnt)){
	return values[index];
    }
    return "";
}

// done
int CGI::getArgCnt(){
    return argCnt;
}

// done
void CGI::error(const std::string &message, int code) {
    std::cout << "{\n\t\"status\": \"error\", \n\t\"message\": \"" 
	      << message <<  "\", \n\t\"code\": " 
	      << code << "\n}" << std::endl;
    exit(0);
}


// done
std::string CGI::getEnvStr(const std::string &key){
    char const* val = getenv(key.c_str());
    return val == NULL ? std::string() : std::string(val);
}



void CGI::clearJSON() {
    json.clear();
}

int CGI::addJSON(const std::string &key, const std::string &val) {
    if (val.compare("") == 0 || key.compare("") == 0) {
	return -1;
    }
    if (json.size() > 0) {
	json += ",\n";
    }
    std::string tmp("");
    tmp += "\"";
    tmp += key;
    tmp += "\"";
    tmp += " : ";
    tmp += val;
    json += tmp;
    return 0;
}

int CGI::addJSON(const std::string &object) {
    if (object.compare("") == 0) {
	return -1;
    }
    if(object.find("{\"") < 2){
	return -1;
    }
    if (json.size() > 0) {
	json += ",\n";
    }
    std::string tmp("");
    tmp += object;    
    json += tmp;
    return 0;


}

void CGI::printJSON() {
    std::cout << "{\n";
    std::cout << json;
    std::cout << "\n}";
    std::cout << std::endl;
}

std::string CGI::getJSON() {
    std::string tmp = "";
    tmp += "{\n";
    tmp += json;
    tmp += "\n}";
    return tmp;
}

void CGI::makeJSON(const std::list<std::string> &queryFields) {    
    clearJSON();
    for (std::list<std::string>::const_iterator it = queryFields.begin();
	 it != queryFields.end(); it++) {
	if (addJSON(*it, get(*it)) < 0) {
	    error("User requested undefined field.", 0);
	}
    }    
}
