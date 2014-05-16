#include "socket.h"
#include <iostream>
#include <sys/types.h> // definitions for system types
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> // needed for internet domain routing
#include <string.h>
#include <netdb.h>
#include <stdio.h> // standard libraries

#include "socketB.h"

Socket::Socket(unsigned short port){
    socket = new UDPSocket(port);
}

Socket::Socket(){
    socket = new UDPSocket();
    unsigned short port = socket.getLocalPort();
    std::string address = socket.getLocalAddress();

}

// needs to recieve the client address pointer
BSONObj Socket::recieve(struct sockaddr_in* cli_addr){
        char* buf[MESSAGE_SIZE]; // server reads input to this buffer

		socket.recvFrom(*buf, MESSAGE_SIZE, &cli_addr)

        BSONObj recieved = mongo::fromjson(buf, MESSAGE_SIZE);

        if(!recieved){
            fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
            return 1;
        }
        return recieved;
}

 void Socket::send(struct sockaddr_in* cli_addr, char * message[]){
		socket.sendTo(message, sizeof(message), (struct sockaddr *)&cli_addr);
 }

void Socket::send(struct sockaddr_in* cli_addr){
        Socket::send(*cli_addr, "Got your message");
}

void Socket::send(struct sockaddr_in* cli_addr, std::string message){
        char *msgChar = new char[message.size()+1]
        msgChar[message.size()]=0;
        memcpy(msgChar,message.c_str(),message.size());
        Socket::send(*cli_addr, msgChar);
}


void Socket::send(struct sockaddr_in* cli_addr, BSONObj BSMessage){
        Socket::send(*cli_addr, BSMessage.jsonString());
 }


void Socket::send(std::list<struct sockaddr_in> SocketList, BSONObj BSMessage){
      		for (std::list<struct sockaddr_in>::iterator cli_addr = broadcast->SocketList.begin(); cli_addr != broadcast->SocketList.end(); cli_addr++){
				Socket::send(*cli_addr, BSMessage);
      		}
 }

Socket::~Socket(){
    delete socket;
}
// dumps error messages when they occur
void Socket::error(char *msg){
	perror(msg);
	exit(1);
}
