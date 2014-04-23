/*
 * This code was orginally written and developed by the Lux Backend Team
 * at Philadelphia Game Lab:
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * - Jake
 */

#include <iostream>
#include <sys/types.h> // definitions for system types
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> // needed for internet domain routing
#include <string.h>
#include <netdb.h>
#include <stdio.h> // standard libraries
#include <list>
#include <queue>
#include "socket.h"

#define MESSAGE_SIZE 256



// called when a system call fails
void error(char *msg){
	perror(msg);
	exit(1);
}
// information on the connection to the database
typedef struct DBC{
};

typedef struct broadcastStruct{
    // socket list
    std::list<struct sockaddr_in> SocketList;
    // message queue
    std::queue<char*> BroadcastQueue;
    Socket socket;
};


void sendUpdatesThread(char * sendFIFO){
    // establish new sending port
    Socket sendSocket;
	sendSocket.init();

	// buffer for messages
    char buf[MAX_BUF];
	FIFO = open(sendFIFO, O_RDONLY);
	while(1){
		read(FIFO, buf, MAX_BUF);
        JSObject message = JSON.parse(buf); // parse to JSON
        // read location from header

        // Test if broadcast or shout, if shout, check location and
        std::list<struct sockaddr_in> SocketList;
        if(message.type == "broadcast"){
            SocketList = HMBL.getSocketLists();
        }else if(message.type == "shout"){
            SocketList = HMBL.getSocketLists(message.location);
        }

        // Strip header from message
        JSObject sendingMessage = message.object;

        // convert message to string
        std::string toBeSent = JSON.stringify(sendingMessage);

		// this is the send command
		sendSocket.send(SocketList, toBeSent);
    }
}

void newConnectionsThread(){

}
int verifyClient(std::string clientID, std::string token){
    return 1;
}

void BattleGroundThread(.....){

    // DO SOMETHING DOCUMENT DATABASE RELATED

    char * shout = "/temp/shout";
    mkfifo(shout, 0666);
    int SP = open(shout, O_WRONLY);
    pthread_create(&BGTID, NULL, (void *) &sendUpdatesThread, (void *) &BGTparam); // spawn BGT

    char * broadcast = "/temp/broadcast";
    mkfifo(broadcast, 0666);
    int BCP  = open(broadcast, O_WRONLY);
    pthread_create(&BGTID, NULL, (void *) &sendUpdatesThread, (void *) &BGTparam); // spawn BGT

    char * newConnection = "/temp/newConnection";
    mkfifo(newConnection, 0666);
    int NCP = open(newConnection, O_WRONLY);
    pthread_create(&BGTID, NULL, (void *) &newConnectionThread, (void *) &BGTparam); // spawn BGT

    Socket recieveSocket;
	recieveSocket.init();

	while(1){
	    // accept clients, who will send in their "User Token"
		socket.recieve(&cli_addr);
		// this will be how we read the "User Token"
		char clientMessage[256] = socket.GetMessage();
		JSObject message = JSON.parse(clientMessage);

		 // if the client checks out
		if(verifyClient(message.clientID, message.Token) > 0){
            int updated = HMBL.updateLocation(cli_addr, message.client_location);
            if(updated > 0){
                if(message.type == "broadcast"){
                    write(BCP, JSON.stringify(message), sizeof(message));
                }else if(message.type == "shout"){
                    write(SP, JSON.stringify(message), sizeof(message));
                }
                write(AP, JSON.stringify(message), sizeof(message)); // send to analyitics

                // UPDATE IN DOCUMENT DB

            }else if(updated == -1){ // not in correct thread
                socket.send(&cli_addr, "{error: \"This is not the thread you are looking for\"}");
            }else if(updated == 0){ // new user
                write(NCP, cli_addr, sizeof(cli_addr));
            }

		}else{
		    // Client Didn't Check out.
            socket.send(&cli_addr, "{error: \"Error Verifying Identity\"}");
		}

	}
}

void battleGroundFunction(struct BGT *BGTParam){
    // spawn new BGT class with the parameters passed in as arguements
    BattleGroundThread BGT(BGTParam.port, BGTParam.DBConnection, BGTParam.BGTIdentity......... );
}

int main(int argc, char *argv[]){
    // open a new socket to listen on
    int port = atoi(argv[1]);
    Socket socket(port); // construct a socket
    socket.init(); // initialize it after to give a chance to change variables

    char * analytics = "/temp/analytics";
    // open an analytics pipe
    mkfifo(analytics, 0666);
    int AP = open(analytics, O_WRONLY);

    // connect to the database
    DBConnection UserDB();

    // connect to the client's online database?
    Verify verify();

    // create the HMBL w/ inner HMBL
    HashMapBasedLocation HMBL();

    // Spawn BGTs
    for(int i=0; i < 1; i++){
        typedef struct BGTID{
            int location[6];
            char name[20];
            int id;
        }BGTIdentity;

        BGTIdentity.location; // read location from config file [x,y,z,x',y',z']
        BGTIdentity.name; // read name from config file
        BGTIdentity.id; // randomly generate ID?

        typedef struct BGTparameters{
            int port;
            struct DBC DBConnection;
            struct BGTID BGTIdentity;
            HashMapBasedLocation* HMBL;
            int AP;

        }BGTparam;

        BGTparam.BGTIdentity = BGTIdentity;
        BGTparam.DBConnection; // A struct holding the database connection string
        BGTparam.AP = AP;

        pthread_t BGTID; // ID for BGT;
        pthread_create(&BGTID, NULL, (void *) &battleGroundThread, (void *) &BGTparam); // spawn BGT

        HMBL.addBGT(BGTIdentity, BGTparams); // Put into HMBL
    }

    struct sockaddr_in cli_addr;
	while(1){
		// accept clients, who will send in their "User Token"
		socket.recieve(&cli_addr);
		// this will be how we read the "User Token"
		char clientMessage[256] = socket.GetMessage();
		JSObject message = JSON.parse(clientMessage);

        // if the client checks out
		if(verifyClient(message.clientID, message.Token) > 0){
            // look up the user location in the database
            BGTID userLocation = UserDB.BGTID(message.clientID);
            // compare that to existing threads
            int clientPort = HMBL.getBGT(userLocation);
            // send the Port that the client needs back to the client
            socket.send(&cli_addr, clientPort);
		}else{
		    // Client Didn't Check out.
            socket.send(&cli_addr, "{error: \"Error Verifying Identity\"}");
		}
	}
	return 0; // should be unreachable
}
