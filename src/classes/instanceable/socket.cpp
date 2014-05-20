#include "socket.h"

Socket::Socket(const unsigned short port) {
    socket = new UDPSocket(port);
    InitSocketInfo();
}

Socket::Socket() {
    socket = new UDPSocket();
    InitSocketInfo();
}

Socket::~Socket() {
    delete socket;
}

// dumps error messages when they occur
void Socket::Error(const char *msg) {
    perror(msg);
    exit(1);
}

// Get the port and address from socket
void Socket::InitSocketInfo() {
    port = socket.getLocalPort();
    address = socket.getLocalAddress();
}

// needs to recieve the client address pointer
BSONObj Socket::Receive(const struct sockaddr_in *cli_addr) {
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

void Socket::Send(const struct sockaddr_in *cli_addr) {
    Send(cli_addr, "Got your message");
}

void Socket::Send(const struct sockaddr_in *cli_addr, const char *message) {
    socket.sendTo(message, sizeof(message), (struct sockaddr *)cli_addr);
}

void Socket::Send(const struct sockaddr_in *cli_addr, std::string message) {
    char *msgChar = new char[message.size() + 1];
    msgChar[message.size()] = '\0';
    memcpy(msgChar, message.c_str(), message.size());
    Send(cli_addr, msgChar);
    delete msgChar;
}

void Socket::Send(const struct sockaddr_in *cli_addr, BSONObj BSMessage) {
    send(cli_addr, BSMessage.jsonString());
 }

void Socket::Send(const std::list<struct sockaddr_in> socketList, 
		  BSONObj BSMessage) {
    for (std::list<struct sockaddr_in>::iterator 
	     cli_addr = broadcast->socketList.begin(); 
	 cli_addr != broadcast->socketList.end(); cli_addr++) {
	send(&cli_addr, BSMessage);
    }
}
