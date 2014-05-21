/*
 * server.cpp - A simple single thread UDP server,
 *              uses LuxSocket to receive and send BSONObj.
 *
 */
#include "luxSocket.h"

#define BUFSIZE 1024

int main() {
    using namespace mongo;
    unsigned short port = 3000;
    socketlibrary::LuxSocket *server = new socketlibrary::LuxSocket(port);
    struct sockaddr_in cli_addr;
    
    while (1) {      
      BSONObj bsonBuf = server->receive(&cli_addr);
      std::string buf = bsonBuf.jsonString();
      cout << "server received " << buf.size() << "bytes: " << buf << endl;
      server->send(&bsonBuf, &cli_addr);
    }
}
