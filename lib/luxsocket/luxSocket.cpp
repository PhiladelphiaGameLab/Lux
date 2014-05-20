#include "luxSocket.h"

namespace socketlibrary {

LuxSocket::LuxSocket(const unsigned short port) {
    socket = new UDPSocket(port);
    InitSocketInfo();
}

LuxSocket::LuxSocket() {
    socket = new UDPSocket();
    InitSocketInfo();
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
    port = socket.getLocalPort();
    address = socket.getLocalAddress();
}

// needs to recieve the client address pointer
BSONObj LuxSocket::receive(const struct sockaddr_in *cli_addr) {
    char buf[MESSAGE_SIZE]; // server reads input to this buffer
    memset(buf, 0, sizeof(buf));

    socket.recvFrom(buf, MESSAGE_SIZE, cli_addr);

    BSONObj received = mongo::fromjson(buf, MESSAGE_SIZE);

    if(!received) {
	fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
	return 1;
    }
    return received;
}

void LuxSocket::send(const struct sockaddr_in *cli_addr) {
    send(cli_addr, "Got your message");
}

void LuxSocket::send(const struct sockaddr_in *cli_addr, const char *message) {
    socket.sendTo(message, sizeof(message), (struct sockaddr *)cli_addr);
}

void LuxSocket::send(const struct sockaddr_in *cli_addr, std::string message) {
    char *msgChar = new char[message.size() + 1];
    msgChar[message.size()] = '\0';
    memcpy(msgChar, message.c_str(), message.size());
    send(cli_addr, msgChar);
    delete msgChar;
}

void LuxSocket::send(const struct sockaddr_in *cli_addr, BSONObj BSMessage) {
    send(cli_addr, BSMessage.jsonString());
 }

void LuxSocket::send(const std::list<struct sockaddr_in> socketList, 
		  BSONObj BSMessage) {
    for (std::list<struct sockaddr_in>::iterator 
	     cli_addr = broadcast->socketList.begin(); 
	 cli_addr != broadcast->socketList.end(); cli_addr++) {
	send(&cli_addr, BSMessage);
    }
}

}
