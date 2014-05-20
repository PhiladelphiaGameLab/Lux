#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <sys/types.h> // definitions for system types
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> // needed for internet domain routing
#include <string.h>
#include <netdb.h>
#include <stdio.h> // standard libraries
#include <bsonobj.h>

#include "socketB.h"


#define MESSAGE_SIZE 256

// instanciable class

class Socket{
    private:
        UDPSocket socket;
        unsigned short port;
        std::string address;
    public:
        /** Default constructor */
        Socket();
        Socket(const unsigned short port);
        virtual ~Socket();

        void init();
        void error(const char* msg);
        BSONObj receive(const struct sockaddr_in *cli_addr);
        void send(const struct sockaddr_in *cli_addr);
        void send(const struct sockaddr_in* cli_addr, const char *message);
        void send(const struct sockaddr_in *cli_addr, std::string message);
        void send(const struct sockaddr_in *cli_addr, BSONObj BSMessage);
        void send(const std::list<struct sockaddr_in> socketList, 
		  BSONObj BSMessage);
	void initSocketInfo();
    protected:
};

#endif // SOCKET_H
