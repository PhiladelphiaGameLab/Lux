#include "luxSocket.h"

namespace socketlibrary {
    // Create new UDP socket server using port
    LuxSocket::LuxSocket(const unsigned short port) {
	try {
	    _socket = new UDPSocket(port);
	}catch(SocketException e){
	    _socket = new UDPSocket(0);
	}
    }

    // Create a new UDP socket client without binding to a port
    LuxSocket::LuxSocket() {
	_socket = new UDPSocket();
    }

    LuxSocket::~LuxSocket() {
	if (_socket) {
	    delete _socket;
	}
    }

    // Dumps error messages when they occur
    void LuxSocket::error(const char *msg) {
	perror(msg);
	exit(1);
    }

    int LuxSocket::receive(void *buf, size_t len, struct sockaddr_in *cliAddr) {
	memset(buf, 0, len);
	return _socket->recvFrom(buf, len, cliAddr);	
    }

    unsigned short LuxSocket::getLocalPort(){
	return _socket->getLocalPort();
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
	try{
		if(bsMessage.isValid()){
			send(bsMessage.jsonString(), cliAddr);
		}
	}catch(exception& e){
		std::cout << e.what() << std::endl;
	}
	

    }

    // Broadcast a BSON object to all clients in list
    void LuxSocket::send(mongo::BSONObj &bsMessage, 
			 std::list<struct sockaddr_in> &socketList) {
	for (std::list<struct sockaddr_in>::iterator cliAddr = socketList.begin(); 
	     cliAddr != socketList.end(); cliAddr++) {
	    send(bsMessage, &(*cliAddr));
	}
    }

    void LuxSocket::send(void *buf, size_t len, struct sockaddr_in *cliAddr) {
	_socket->sendTo(buf, len, cliAddr);
    }
}
