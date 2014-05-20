/*
 * server.cpp - A simple single thread UDP server,
 *              uses LuxSocket to receive and send BSONObj.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "luxSocket.h"

#define BUFSIZE 1024

int main() {
    unsigned short port = 3000;
    socketlibrary::LuxSocket *server = new socketlibrary::LuxSocket(port);
    struct sockaddr_in cli_addr;
    mongo::BSONObj *bsonBuf;
    

    while (1) {      
      bsonBuf = NULL;
      server->receive(bsonBuf, &cli_addr);
      //printf("server received %lu bytes: %s\n", strlen(buf),  buf);
      server->send(bsonBuf, &cli_addr);
    }
}
