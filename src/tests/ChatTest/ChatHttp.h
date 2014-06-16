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
    private:
    	void AddUser(std::string uName, std::string givenIP, std::string givenPort);
        void AddGroup(std::string grName);
		void DeleteUser(std::string uName);
		void AddEdge(std::string uName1, std::string uName2);
		void DeleteEdge(std::string uName1, std::string uName2);
    public:
        ChatHttp();
    protected:
};

#endif // CHATHTTP_H