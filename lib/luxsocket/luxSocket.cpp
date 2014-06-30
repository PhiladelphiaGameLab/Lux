#include "luxSocket.h"

namespace socketlibrary {
    // Server opens a new UDP socket at port
    LuxSocket::LuxSocket(const unsigned short port) {
	_socket = new UDPSocket(port);
	initSocketInfo();
    }

    // Server opens a new UDP socket
    // Default port is 3000
    LuxSocket::LuxSocket() {
	_socket = new UDPSocket(3000);
        initSocketInfo();
    }

    LuxSocket::~LuxSocket() {
	delete _socket;
    }

    // Dumps error messages when they occur
    void LuxSocket::error(const char *msg) {
	perror(msg);
	exit(1);
    }

    // Gets the port and address from socket
    void LuxSocket::initSocketInfo() {
	_port = _socket->getLocalPort();
	_address = _socket->getLocalAddress();
    }

    int LuxSocket::receive(void *buf, size_t len, struct sockaddr_in *cliAddr) {
	memset(buf, 0, len);
	return _socket->recvFrom(buf, len, cliAddr);	
    }

    // Server receives data from client
    mongo::BSONObj LuxSocket::receive(struct sockaddr_in *cliAddr) {
	char buf[MESSAGE_SIZE]; // server reads input to this buffer

	memset(buf, 0, MESSAGE_SIZE);
	_socket->recvFrom(buf, MESSAGE_SIZE, cliAddr);

	return mongo::fromjson(buf);
    }

    // Send default message back
    void LuxSocket::send(struct sockaddr_in *cliAddr) {
	send("Got your message", cliAddr);
    }

    // Send a char array to client 
    void LuxSocket::send(const char *message, struct sockaddr_in *cliAddr) {
	_socket->sendTo(message, strlen(message), cliAddr);
    }

    // Send a string to client
    void LuxSocket::send(const std::string &message, struct sockaddr_in *cliAddr) {
	char *msgChar = new char[message.size() + 1];
	memcpy(msgChar, message.c_str(), message.size());
	send(msgChar, cliAddr);
	delete msgChar;
    }

    // Send a BSON object to client
    void LuxSocket::send(mongo::BSONObj &bsMessage, struct sockaddr_in *cliAddr) {
	send(bsMessage.jsonString(), cliAddr);
    }

    // Broadcast a BSON object to all clients in list
    void LuxSocket::send(mongo::BSONObj &bsMessage, 
			 std::list<struct sockaddr_in> &socketList) {
	for (std::list<struct sockaddr_in>::iterator cliAddr = socketList.begin(); 
	     cliAddr != socketList.end(); cliAddr++) {
	    send(bsMessage, &(*cliAddr));
	}
    }
}
