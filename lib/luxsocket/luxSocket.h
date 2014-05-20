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
#include <bsonobj.h>

#include "socket.h"

#define MESSAGE_SIZE 256

namespace socketlibrary {
// instanciable class

class LuxSocket{
    private:
        UDPSocket socket;
        unsigned short port;
        std::string address;
    public:
        /** Default constructor */
        LuxSocket();
        LuxSocket(const unsigned short port);
        virtual ~LuxSocket();

        void init();
        void error(const char *msg);
        BSONObj receive(const struct sockaddr_in *cli_addr);
        void send(const struct sockaddr_in *cli_addr);
        void send(const struct sockaddr_in* cli_addr, const char *message);
        void send(const struct sockaddr_in *cli_addr, std::string &message);
        void send(const struct sockaddr_in *cli_addr, BSONObj &BSMessage);
        void send(const std::list<struct sockaddr_in> &socketList, 
		  BSONObj &BSMessage);
	void initSocketInfo();
    protected:
};

}

#endif // LUXSOCKET_H
