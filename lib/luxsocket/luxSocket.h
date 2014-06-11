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

#define MESSAGE_SIZE 1024

namespace socketlibrary {

class LuxSocket{
    private:
        UDPSocket *socket;
        unsigned short port;
        std::string address;
    public:
        /** Default constructor */
        LuxSocket();
        LuxSocket(const unsigned short port);
        virtual ~LuxSocket();

        void init();
        void error(const char *msg);
	void receive(char *buf, struct sockaddr_in *cli_addr);
	mongo::BSONObj receive(struct sockaddr_in *cli_addr);
        void send(struct sockaddr_in *cli_addr);
        void send(const char *message, struct sockaddr_in* cli_addr);
        void send(const std::string &message, struct sockaddr_in *cli_addr);
        void send(mongo::BSONObj &BSMessage, struct sockaddr_in *cli_addr);
        void send(mongo::BSONObj &BSMessage,
		  std::list<struct sockaddr_in> &socketList);
	void initSocketInfo();
    protected:
};

}

#endif // LUXSOCKET_H
