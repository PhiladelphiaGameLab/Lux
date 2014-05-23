#include "sendupdate.h"

void SendUpdate::spawn(struct sendUpdateArgs params_in) {
	
	if(params_in.writeToDb > 0) {
	    mkfifo("/temp/pipe", 0666); // opens pipe
	    //int DBW = open(dbWriter, O_WRONLY);
	    //struct pipe dbP;
	    //dbP.pipe = DBW;
	    //pthread_create(&BGTID, NULL, (void *) &SendUpdate::dbWriter, (void *) &dbP); // spawn dbWriter thread
	}

	// establish new sending port
	Socket sendSocket(params_in.port); // create a socket object
	sendSocket.init(); // initialize/open the socket

	int FIFO = open("/temp/pipe", O_RDONLY);

	struct s_SUTMessage piped;
	
	while(1) {
		read(FIFO, piped, MAX_BUF);
		
		// loops through each bucket in the linkedList of buckets from HMBL
		for (Bucket currentBucket = piped.Bucket; currentBucket -> Next != null; currentBucket = currentBucket -> Next) {
			
			// loops through each node in the linkedList of sockets in each bucket
			for (Node currentNode = currentBucket.Head; currentNode -> Next != null; currentNode = currentNode -> Next) {
				// TODO: lock
				pthread_mutex_lock(currentNode.Lock);
				sockaddr_in cli_addr = currentNode.socket;
				Socket::send(&cli_addr, piped.message);
    				// TODO: unlock
    				pthread_mutex_unlock(currentNode.Lock);
 			}
		}

	} 
     //   	// break piped into sockets & object
     //   	// send to socket list via socket class
     //   	sendSocket.send(piped.SocketList, piped.message);
     //   	// write message to database thread
     //   	if(params_in.writeToDb > 0){
     //       		write(dbWriter, piped.message, sizeof(piped.message));
     //   	}
    	// }	
}
