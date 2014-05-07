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

void mongoConnect() {
  mongo::DBClientConnection c;
  c.connect("localhost");
  return c;
}


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
    mongoConnect();

    try {
        mongo::DBClientConnection c = mongoConnect();
        std::cout << "connected ok" << std::endl;
      } catch( const mongo::DBException &e ) {
        std::cout << "caught " << e.what() << std::endl;
      }
      return EXIT_SUCCESS;
    }

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

}

int MainMessagePassingServer::start(){
    struct sockaddr_in cli_addr;
	while(1){

	    /*
	    Why do we establish a socket for a single message,
	    Why don't we have an HTTP request or something so the client only has to establish a single socket connection
	    */



		// accept clients, who will send in their "User Token"
		socket.recieve(&cli_addr);
		// this will be how we read the "User Token"
		json_t clientMessage = socket.GetMessage();

        // if the client checks out
		if(verifyClient(message.clientID, message.Token) > 0){
            // look up the user location in the database
            BGTID userLocation = UserDB.BGTID(message.clientID);
            // compare that to existing threads
            int clientPort = HMBS.getBGT(userLocation);
            // send the Port that the client needs back to the client
            socket.send(&cli_addr, clientPort);
		}else{
		    // Client Didn't Check out.
            socket.send(&cli_addr, "{error: \"Error Verifying Identity\"}");
		}
	}
	return 0;
}


