#ifndef CHATHTTP_H
#define CHATHTTP_H

#include <curlpp/cURLpp.hpp>
#include <cstdlib>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"

#include "../../classes/instanceable/CGI.h"
#include "../../classes/static/MD5.h"
#include "../../classes/static/Authenticate.h"
//path to be determined later
//#include "socket.h"

class ChatHttp{
	public:
    	static void AddNode(std::string uName, std::string givenIP, std::string type);
		static void DeleteNode(std::string uName);
		static void AddEdge(std::string uName1, std::string uName2);
		static void DeleteEdge(std::string uName1, std::string uName2);
		static void GetInfo(std::string uName, std::string type);
		static void UpdateInfo(std::string uName, std::string type, std::string value);
		static void FindNode(std::string type, std::string value);
	protected:
    private:
};

#endif // CHATHTTP_H