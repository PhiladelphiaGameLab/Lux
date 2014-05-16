#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <iostream>
#include <cstdlib>
#include "bsonobj.h"


#include "../classes/instanceable/socketB.h"

void* socketSEND(void*){

    std::cout << "Send Started" << std::endl;
    struct sockaddr_in* srv_addr;
    /* STILL NEED TO SEND TO A SOCKET THAT I DON'T KNOW YET?!" */
/*
    Socket sendSocket;

	int i;
    for(i=0; i<10; i++){
        sendSocket.send(srv_addr, "Test Message " << i);
        std::cout << " Message : " << sendSocket.recieve(&srv_addr).toString() << std::endl;
    }
*/
}

void* socketRECIEVE(void*){

    std::cout << "Recieve Started" << std::endl;
    struct sockaddr_in* cli_addr;

    Socket recieveSocket;

    for(;;){
        std::cout << " Message : " << recieveSocket.recieve(&cli_addr).toString() << std::endl;

        recieveSocket.send(cli_addr, "Message Recieved");
    }

}


int main(int argc, char *argv[]){


    // break off a thread
    pthread_t recID;
    pthread_t sendID;
    pthread_create(&recID, NULL, socketRECIEVE, NULL);
    pthread_create(&sendID, NULL, socketSEND, NULL);

    std::cout << "Main Thread" << std::endl;

    // break off a second thread

    // send messages to the first socket from the second

}
