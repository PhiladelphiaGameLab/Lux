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

#include "mainmessagepassingserver.h"
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

MainMessagePassingServer::MainMessagePassingServer(int port, std::string configFile){
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
}

MainMessagePassingServer::~MainMessagePassingServer()
{
    //dtor
}

MainMessagePassingServer::MainMessagePassingServer(const MainMessagePassingServer& other)
{
    //copy ctor
}

MainMessagePassingServer& MainMessagePassingServer::operator=(const MainMessagePassingServer& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}

int MainMessagePassingServer::createThreads(int numThreads){
    // Spawn BGTs
    for(int i=0; i < numThreads; i++){
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
        pthread_create(&BGTID, NULL, (void *) &BattleGround::createBGT, (void *) &BGTparam); // spawn BGT

        HMBL.addBGT(BGTIdentity, BGTparams); // Put into HMBL
    }
    return 0;
}

int MainMessagePassingServer::start(){
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
	return 0;
}


