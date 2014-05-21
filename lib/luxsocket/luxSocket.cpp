#include "luxSocket.h"

namespace socketlibrary {

LuxSocket::LuxSocket(const unsigned short port) {
    socket = new UDPSocket(port);
    initSocketInfo();
}

LuxSocket::LuxSocket() {
    socket = new UDPSocket();
    initSocketInfo();
}

LuxSocket::~LuxSocket() {
    delete socket;
}

// dumps error messages when they occur
void LuxSocket::error(const char *msg) {
    perror(msg);
    exit(1);
}

// Get the port and address from socket
void LuxSocket::initSocketInfo() {
    port = socket->getLocalPort();
    address = socket->getLocalAddress();
}

// needs to recieve the client address pointer

void LuxSocket::receive(mongo::BSONObj *bsonBuf, struct sockaddr_in *cli_addr) {
    char buf[MESSAGE_SIZE]; // server reads input to this buffer

    memset(buf, 0, MESSAGE_SIZE);

    socket->recvFrom(buf, MESSAGE_SIZE, cli_addr);

    *bsonBuf = mongo::fromjson(buf);
}

void LuxSocket::send(struct sockaddr_in *cli_addr) {
    send("Got your message", cli_addr);
}

void LuxSocket::send(const char *message, struct sockaddr_in *cli_addr) {
    socket->sendTo(message, strlen(message), cli_addr);
}

void LuxSocket::send(std::string &message, struct sockaddr_in *cli_addr) {
    char *msgChar = new char[message.size() + 1];
    msgChar[message.size()] = '\0';
    memcpy(msgChar, message.c_str(), message.size());
    send(msgChar, cli_addr);
    delete msgChar;
}

void LuxSocket::send(mongo::BSONObj &BSMessage, struct sockaddr_in *cli_addr) {
    send(BSMessage.jsonString(), cli_addr);
}

void LuxSocket::send(mongo::BSONObj &BSMessage, 
		     std::list<struct sockaddr_in> &socketList) {
    for (std::list<struct sockaddr_in>::iterator 
	     cli_addr = broadcast->socketList.begin(); 
	 cli_addr != broadcast->socketList.end(); cli_addr++) {
        send(BSMessage, &cli_addr);
    }
}
}
