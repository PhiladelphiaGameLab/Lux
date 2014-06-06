/*
read the message from the socket
get the access token & EUID
Authenticate EUID against Access Token
	strip access token
	add timestamp
	check for _id
		insert into mongo
	get location from message
	check if EU_doc || cli_obj_doc
		EU_doc:
			update in HMBL
		get the bucket 
		update the bucket in the document
		get the relevent socket list of the new bucket
		send to send updates thread
		append the client_socket
 */

#include "battleground.h"

void BattleGround::spawn(void *param_in){

    // this pipe stuff should be right:

    // create pipe to send updates on
    const char *pipeLocation = "/temp/pipe"; // needs to be unique somehow
    if(mkfifo(pipeLocation, 0666) == 0){ // creates the pipe in the file system
        int pipe = open(pipeLocation, O_WRONLY); // open  the pipe for writing

        // so should the idea of this pthread

        s_SUT param;
        param.pipeLocation = pipeLocation;

       // pthread_create(&BGTID, NULL, (void *) &SendUpdate::createSUT, (void *) &param); // spawn sendUpdates thread

    }


    // construct a HMBL
    //locbasedhashmap HMBL;
    locbasedhashmap HMBL(thredSizeX,threadSizeY,pipeLocation);

    Socket socket(param_in.port); // create a socket object
	socket.init(); // initialize/open the socket


    // will need to pass the socket that was opened back to the
    // spawn BGT so that it can use that later for redirection

	while(1){
	    sockaddr_in cli_addr;
	    // accept clients, who will send in their update
		BSONObj message = socket.receive(&cli_addr);

        // get accessToken from BSONObj message
        std::string accessToken = message["sender"]["accessToken"].String(); // this should be as easy as this- but might not be.
        // get EUID from BSONObj message
        std::string EUID = message["sender"]["EUID"].String();


        // authenticate message
        if(Authenticate::authenticateAccessToken(accessToken, EUID)){
            // get location from message
            int location[0] = atoi(message["object"]["location"]["x"].String().c_str());
            int location[1] = atoi(message["object"]["location"]["y"].String().c_str());
            int radius = atoi(message["sender"]["radius"].String().c_str());

	    //Updte clients location in HMBL
	    HMBL.update(cli_addr,EUID,location[0],location[1]);
		
		
            // query HMBL for socket list
            std::list<struct sockaddr_in> SocketList = HMBL.getClients(location[0],location[1],radius);// need to pass in cli_addr, location, and radius

            // pass message to undecided server logic class that client will fill in
            // sadly this might be unavoidable
            // :-(

	    //Strip message header
	    BSONobj strippedMessage = message["object"];


	    //Create  structure
	    s_SUTMessage pipeStruct;
	    pipeStruct.message = strippedMessage;
	    pipeStruct.SocketList = SocketList;
	    
            // pipe updates to send updates thread
            write(pipe, pipeStruct, sizeof(pipeStruct));

        }

	}
}
