#include "CGI.h"

// done
std::string CGI::decodeString(const std::string &str) {
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

CGI::CGI(const std::string contentType) : _argCnt(0){
    std::cout << "Content-type: " << contentType << " charset=utf-8\n\n";
    // done
    // Initialize private variables
    for (int pos = 0; pos < MAX_ARGS; pos++){
        _names[pos] = _values[pos] = "";
    }
    // Read environment variables
    std::string requestMethod = CGI::getEnvStr("REQUEST_METHOD");
    std::string queryString = CGI::getEnvStr("QUERY_STRING");
    std::string contentLength = CGI::getEnvStr("CONTENT_LENGTH");
    
    int queryLength = 0;

    // Check request_method variable
    if (requestMethod.compare("")  == 0)
	return;

    if (requestMethod.compare("GET") == 0) {
	// Handle GET requests
	queryLength = queryString.length();
    }
    else if (requestMethod.compare("POST") == 0) {
	// Handle POST requests
	queryLength = atoi(contentLength.c_str());
	
	char *buf = new char[MAX_LENGTH];	    
	fread(buf, 1, queryLength, stdin);
	queryString = buf;
    }
    else {
	// Ignore on other requests
	return;
    }

    /*
    int post_length;
    char *post_string;
    // Handle POST requests

    if (contentLength != ""){
	postLength = atoi(contentLength.c_str());
	postString = (char *)malloc(postLength);
	if (postString != "") {
	    for (int pos = 0; pos < post_length; pos++){
		postString[pos] = fgetc(stdin);
	    }
	    _jsonIn = mongo::fromjson(postString, postLength);
	}
	free(postString);
    }
    */

    // Separate queryString into arguments
    int startName, endName, startValue, endValue = -1;
    while (endValue < queryLength){
	// Find argument name	
	startName = endName = endValue + 1;
	while ((endName < queryLength) && queryString[endName] != '=') {
	    endName++;
	}

	// Copy and decode name string
	std::string xd = queryString.substr(startName, endName - startName);
	_names[_argCnt] = decodeString(xd);

	// Find argument value
	startValue = endValue = endName + 1;
	while ((endValue < queryLength) && queryString[endValue] != '&') {
	    endValue++;
	}

	// Copy and decode value string
	std::string md = queryString.substr(startValue, endValue - startValue);
	_values[_argCnt] = decodeString(md);

	_argCnt++;
    }
}

// done
mongo::BSONElement CGI::get(const std::string &name, bool bson) {
    if (_jsonIn.hasElement(name.c_str())) {
	return _jsonIn[name];
    }
    return mongo::BSONElement();
}

std::string CGI::get(const std::string &name) {
    int i;
    for (i=0; i < _argCnt; i++) {
	if (name.compare(_names[i]) == 0) {
	    return _values[i];
	}
    }
    
    if(_jsonIn.hasElement(name.c_str())) {
	std::string ele = _jsonIn[name].toString(false);
	if (ele[0] == '\"') {
	    ele = ele.substr(1, ele.size() - 2);
	}
	return ele;
    }
    return "";
}

// done
std::string CGI::getName(int index) {
    // Lookup argument by location
    if ((index >= 0) && (index < _argCnt)) {
	return _names[index];
    }
    return "";
}

// done
std::string CGI::getValue(int index) {
    // Lookup argument by location
    if ((index >= 0) && (index < _argCnt)){
	return _values[index];
    }
    return "";
}

// done
int CGI::getArgCnt(){
    return _argCnt;
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
    _json.clear();
}

int CGI::addJSON(const std::string &key, const std::string &val) {
    if (val.compare("") == 0 || key.compare("") == 0) {
	return -1;
    }
    if (_json.size() > 0) {
	_json += ",\n";
    }
    std::string tmp("");
    tmp += "\"";
    tmp += key;
    tmp += "\"";
    tmp += " : ";
    tmp += val;
    _json += tmp;
    return 0;
}

int CGI::addJSON(const std::string &object) {
    if (object.compare("") == 0) {
	return -1;
    }
    if(object.find("{\"") < 2){
	return -1;
    }
    if (_json.size() > 0) {
	_json += ",\n";
    }
    std::string tmp("");
    tmp += object;    
    _json += tmp;
    return 0;


}

void CGI::printJSON() {
    std::cout << "{\n";
    std::cout << _json;
    std::cout << "\n}";
    std::cout << std::endl;
}

std::string CGI::getJSON() {
    std::string tmp = "";
    tmp += "{\n";
    tmp += _json;
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
