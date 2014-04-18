#include <iostream>

int main(int argc, char *argv[]){
	
	// establish a socket to listen on using a cmd line arg
	
	// spawn BGT(s)
	// array of BGT ports 
	while(1){
		// accept clients, send them the port address they should be on

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
	while(1){
		if(!*broadcast->BroadcastQueue.empty()){ // if there is something in the broadcast queue 
			for (std::list<TCPServerSocket>::iterator socket = *broadcast->SocketList.begin(); socket != *broadcast->SocketList.end(); socket++){
				*socket->send(*broadcast->BroadcastQueue.front());
			}
			*broadcast->BroadcastQueue.pop();
		}
		// otherwise we can probably give up our processor time. 
	}
}
