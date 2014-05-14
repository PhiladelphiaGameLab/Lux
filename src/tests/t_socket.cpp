#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <iostream>
#include <cstdlib>


#include "../classes/instanceable/socket.h"

void* socketSEND(void*){

    std::cout << "Send Started" << std::endl;

    // create a socket
    // initialize the socket
    // send messages to the first socket from the second

    // read from a random dictionary file and send the message to the port

    // print out the response message
}

void* socketRECIEVE(void*){

    std::cout << "Recieve Started" << std::endl;
    // create a second socket
    // initialize the second socket
    // print out the messages recieved

    // recieve the message and send a response


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
