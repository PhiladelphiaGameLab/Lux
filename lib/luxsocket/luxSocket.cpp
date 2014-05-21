#include "luxSocket.h"

namespace socketlibrary {
  
  /*
   * Server opens a new UDP socket at port
   */
  LuxSocket::LuxSocket(const unsigned short port) {
    socket = new UDPSocket(port);
    initSocketInfo();
  }

  /*
   * Server opens a new UDP socket
   */
  LuxSocket::LuxSocket() {
    socket = new UDPSocket();
    initSocketInfo();
  }

  LuxSocket::~LuxSocket() {
    delete socket;
  }

  // Dumps error messages when they occur
  void LuxSocket::error(const char *msg) {
    perror(msg);
    exit(1);
  }

  // Gets the port and address from socket
  void LuxSocket::initSocketInfo() {
    port = socket->getLocalPort();
    address = socket->getLocalAddress();
  }

  // Server receives data from client
  mongo::BSONObj LuxSocket::receive(struct sockaddr_in *cli_addr) {
    char buf[MESSAGE_SIZE]; // server reads input to this buffer

    memset(buf, 0, MESSAGE_SIZE);
    socket->recvFrom(buf, MESSAGE_SIZE, cli_addr);

    return mongo::fromjson(buf);
  }
  
  // Send default message back
  void LuxSocket::send(struct sockaddr_in *cli_addr) {
    send("Got your message", cli_addr);
  }
  
  // Send a char array to client 
  void LuxSocket::send(const char *message, struct sockaddr_in *cli_addr) {
    socket->sendTo(message, strlen(message), cli_addr);
  }

  // Send a string to client
  void LuxSocket::send(const std::string &message, struct sockaddr_in *cli_addr) {
    char *msgChar = new char[message.size() + 1];
    memcpy(msgChar, message.c_str(), message.size());
    send(msgChar, cli_addr);
    delete msgChar;
  }

  // Send a BSON object to client
  void LuxSocket::send(mongo::BSONObj *BSMessage, struct sockaddr_in *cli_addr) {
    send(BSMessage->jsonString(), cli_addr);
  }

  // Broadcast a BSON object to all clients in list
  void LuxSocket::send(mongo::BSONObj *BSMessage, 
		     std::list<struct sockaddr_in> &socketList) {
    for (std::list<struct sockaddr_in>::iterator cli_addr = socketList.begin(); 
	 cli_addr != socketList.end(); cli_addr++) {
      send(BSMessage, &(*cli_addr));
    }
  }
}
