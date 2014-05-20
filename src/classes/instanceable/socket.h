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

        void Init();
        void Error(const char* msg);
        BSONObj Receive(const struct sockaddr_in *cli_addr);
        void Send(const struct sockaddr_in *cli_addr);
        void Send(const struct sockaddr_in* cli_addr, const char *message);
        void Send(const struct sockaddr_in *cli_addr, std::string message);
        void Send(const struct sockaddr_in *cli_addr, BSONObj BSMessage);
        void Send(const std::list<struct sockaddr_in> socketList, 
		  BSONObj BSMessage);
	void InitSocketInfo();
    protected:
};

#endif // SOCKET_H
