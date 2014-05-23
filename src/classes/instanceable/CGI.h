#ifndef CGI_H
#define CGI_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <list>

#include "mongo/bson/bsonobj.h"
#include "mongo/bson/bsonelement.h"
#include "mongo/db/json.h"

// instanciable class

class CGI{
    public:
        CGI();

	// Returns a enviroment string
        static std::string getEnvStr(const std::string &key);

	// Decodes a value field of query string 
        std::string decode_string(const std::string &str);

	// Returns the value of a field in string format
        std::string get(const std::string &name);

	// Returns the value of a field in BSONElement format
	mongo::BSONElement get(const std::string &name, bool bson);

	// Returns a field name from name arrary.
	// Returns empty string if index is invalid.
        std::string getName(int index);

	// Returns value of a field from value array.
	// Returns empty string if index is invalid.
        std::string getValue(int index);

	// returns arguments counter
        int getArgCnt();
	
	// Prints error message in JSON format
        void error(const std::string &message, int code);

        // Clear json string
        void clearJSON();

        // Add a json key/value pair into json string.
        // Returns -1 if val is empty
        int addJSON(const std::string &key, const std::string &val);

        // Print json string to stdout
        void printJSON();

	// Make a new JSON object from the query list
	void makeJSON(const std::list<std::string> &queryFields);

	// Returns json string
	std::string getJSON();
    protected:
    private:
        static const int MAX_ARGS = 10;
        int argCnt;
        std::string names[MAX_ARGS];
        std::string values[MAX_ARGS];
        mongo::BSONObj JSONin = mongo::fromjson("");
	std::string json;
};

#endif // CGI_H
