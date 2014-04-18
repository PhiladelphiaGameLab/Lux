#include <iostream>
#include <sys/types.h> // definitions for system types
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> // needed for internet domain routing
#include <string.h>
#include <netdb.h>
#include <stdio.h> // standard libraries

#define MESSAGE_SIZE 256

// called when a system call fails
void error(char *msg){
	perror(msg);
	exit(1);
}



int main(int argc, char *argv[]){
	if (argc < 2) {
      		std::cout << "ERROR, no port provided" << std::endl;
      		exit(0);
   	}

	int newsockfd, sock;
	int clilen; // stores the size of the client
	socklen_t fromlen; // number of characters read/written
	
	char buffer[MESSAGE_SIZE]; // server reads input to this buffer
 	

	struct sockaddr_in serv_addr, cli_addr; // server/client addresses

	if(socket(AF_INET, SOCK_DGRAM, 0) < 0){
		error("Error Opening Socket");
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = hton(atoi(argv[1]));
	// establish a socket to listen on using a cmd line arg	
	if (bind(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){ 
      		 error("binding");
	}
	// spawn BGT(s)
	// create an array(?) of BGT ports 
	
	fromlen = sizeof(struct sockaddr_in);
	while(1){
		// accept clients, send them the port address they should be on
		if(n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&cliaddr,&fromlen) < 0){
			error("Could not recieve on socket");
		}
		write(1,"Received a datagram: ",21);
       		write(1,buf,n);

		if(n = sendto(sock,"Got your message\n",17,0,(struct sockaddr *)&cli_addr,fromlen) < 0){
			error("Could not send to the socket");
		}
		// clients should send their client ID/token to establish identity
		
		// create a hash table of clients/BGT 
	}
	return 0;
}

void battleGroundThread(struct *BGT){
	// initialize linked list of sockets on connection
	// initialize the broadcast queue
	// spawn the broadcast thread
	
	// open listening port
	// update the Port# in the BGT struct

	while(1){
		// listen on port for clients
		// update the Socket List if the client is new
		// push any messages from clients into Broadcast queue
	}
}

void broadcastThread(struct broadcast){
	while(1){/*
		if(!*broadcast->BroadcastQueue.empty()){ // if there is something in the broadcast queue 
			for (std::list<TCPServerSocket>::iterator socket = *broadcast->SocketList.begin(); socket != *broadcast->SocketList.end(); socket++){
				*socket->send(*broadcast->BroadcastQueue.front());
			}
			*broadcast->BroadcastQueue.pop();
		}
		// otherwise we can probably give up our processor time. 
	*/	}
}
