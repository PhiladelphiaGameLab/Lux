#ifndef LUXSOCKET_H
#define LUXSOCKET_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"
#include "mongo/db/json.h"

#include "socket.h"

#define MESSAGE_SIZE 65507 // UDP packet maximum size

namespace socketlibrary {

    class LuxSocket{
	public:
        //Default constructor
        LuxSocket();
        LuxSocket(const unsigned short port);
        ~LuxSocket() {delete _socket;};

        void error(const char *msg);
	int receive(void *buf, size_t len, struct sockaddr_in *cliAddr);
	unsigned short getLocalPort();
	mongo::BSONObj receive(struct sockaddr_in *cliAddr);
        void send(struct sockaddr_in *cliAddr);	
        void send(const char *message, struct sockaddr_in* cliAddr);
        void send(const std::string &message, struct sockaddr_in *cliAddr);
        void send(mongo::BSONObj &bsMessage, struct sockaddr_in *cliAddr);
        void send(mongo::BSONObj &bsMessage,
		  std::list<struct sockaddr_in> &socketList);
	void send(void *buf, size_t len, struct sockaddr_in *cliAddr);

	unsigned short getPortNum() {
	    if (_socket) {
		return _socket->getLocalPort();
	    }
	    else {
		return 0;
	    }
	};
	std::string getAddress() {
	    if (_socket) {
		return _socket->getLocalAddress();
	    }
	    else {
		return "";
	    }
	}

	private:
        UDPSocket *_socket;
    };
}

#endif // LUXSOCKET_H
