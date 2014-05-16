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


#define MESSAGE_SIZE 256

// instanciable class

class Socket{
    public:
        /** Default constructor */
        Socket(int port);
        Socket();
        void init();
        void error(char* msg);
        BSONObj recieve(struct sockaddr_in* cli_addr);
        void send(struct sockaddr_in* cli_addr);
        void send(struct sockaddr_in* cli_addr, BSONObj BSMessage);
        void send(std::list<struct sockaddr_in> SocketList, BSONObj BSMessage);
        void send(struct sockaddr_in* cli_addr, std::string message);
        void Socket::send(struct sockaddr_in* cli_addr, char * message[]);
        virtual ~Socket();
        UDPSocket socket;
        unsigned short port;
        std::string address;
    protected:
    private:
};

#endif // SOCKET_H
