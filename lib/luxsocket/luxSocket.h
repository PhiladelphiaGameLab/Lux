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
        virtual ~LuxSocket();

        void init();
        void error(const char *msg);
	void receive(char *buf, size_t len, struct sockaddr_in *cliAddr);
	mongo::BSONObj receive(struct sockaddr_in *cliAddr);
        void send(struct sockaddr_in *cliAddr);
        void send(const char *message, struct sockaddr_in* cliAddr);
        void send(const std::string &message, struct sockaddr_in *cliAddr);
        void send(mongo::BSONObj &bsMessage, struct sockaddr_in *cliAddr);
        void send(mongo::BSONObj &bsMessage,
		  std::list<struct sockaddr_in> &socketList);
	void initSocketInfo();
	unsigned short getPortNum() {
	    return _port;
	};
	std::string getAddress() {
	    return _address;
	}

	private:
        UDPSocket *_socket;
        unsigned short _port;
        std::string _address;
    };
}

#endif // LUXSOCKET_H
