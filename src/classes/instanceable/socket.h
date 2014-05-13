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
        int main();
        virtual ~Socket();
        unsigned short Getport() { return port; }
        void Setport(unsigned short val) { port = val; }
        short Getsin_family() { return sin_family; }
        void Setsin_family(short val) { sin_family = val; }
        unsigned short Getsin_port() { return sin_port; }
        void Setsin_port(int val) { sin_port = (htons(val)); }
        unsigned long Gets_addr() { return s_addr; }
        void Sets_addr(unsigned long val) { s_addr = val; }
        int Getmessage_size() { return message_size; }
        void Setmessage_size(int val) { message_size = val; }
        static int getOpenPort();
    protected:
    private:
        unsigned short port;
        short sin_family;
        u_short sin_port;
        unsigned long s_addr;
        int message_size;
        int sockfd;
        int newsockfd;
        int n;
        int clilen; // stores the size of the client
        socklen_t fromlen; // number of characters read/written
        char buf[MESSAGE_SIZE]; // server reads input to this buffer
        char message[MESSAGE_SIZE];
        struct sockaddr_in serv_addr; // server addresses
        struct sockaddr_in cli_addr; // client addresses
};

#endif // SOCKET_H
