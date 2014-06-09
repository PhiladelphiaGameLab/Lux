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
#include "HMBL.h"

#include <ctime.h>

void BattleGround::spawn(struct s_bgt_params_in  *param_in){


 	//connect to the database
    	mongo::DBClientConnection c;
    	c.connect("localhost");

    // this pipe stuff should be right:

    // create pipe to send updates on

        int pipe = open(param_in.pipe_w, O_WRONLY); // open  the pipe for writing

        s_SUT param;
        param.pipeLocation = pipeLocation;


    // construct a HMBL
    //locbasedhashmap HMBL;
   HMBL Map(mapSizeX,mapSizeY,thredSizeX,threadSizeY);

    uint16_t portNo = getNewPort();
     c.update((DATABASE_NAME.COLLECTION_NAME,QUERY(),BSON("port"<<portNo),true);


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
        	
        	
            //timestamp (Current time of the system)
	    time_t timestamp = time(NULL); 
            char* t_stamp = ctime(&timestamp); 	
        	
            //Strip Access Token
	    BSONobj strippedEUIDMessage = message["sender"]["EUID"];
		
	    //Strip message header
	    BSONobj strippedObjectMessage = message["object"];
	    
	    mongo::BSONObjBuilder builder;
	    builder.appendElements(strippedEUIDMessage);
	    builder.appendElements(strippedObjectMessage);
	    builder.append("time",t_stamp);
	    BSONobj completeMessage = builder.obj();
	
	
	   //get the id of the message
	   BSONobj id = completeMessage["_id"];
	   
           //Check for id (Maybe cursor is needed)
           if(!c.find(DATABASE_NAME.COLLECTION_NAME,QUERY("_id"<<id)))
           {
           	//insert into MongoDB
           	c.insert(DATABASE_NAME.COLLECTION_NAME,completeMessage);
           }
        	
            // get location from message
            int location[0] = atoi(message["object"]["location"]["x"].String().c_str());
            int location[1] = atoi(message["object"]["location"]["y"].String().c_str());
            int radius = atoi(message["sender"]["radius"].String().c_str());

	
	    //Get the values of EU_DOC and cli_obj_doc
	    std::bool EU_DOC	  = completeMessage["sender"]["EU_DOC"].String();
	    std::bool cli_obj_doc = completeMessage["sender"]["cli_obj_doc"].String();

	    if(EU_DOC || cli_obj_doc)
	    {
	    //Updte clients location in HMBL
	    Map.update(cli_addr,EUID,location[0],location[1],radius);
	    }
	     	
	     	
	    //get bucket ID
	    //int bgt_id = Map.getBucket(location[0],location[1],mapSizeX,mapSizeY,thredSizeX,threadSizeY);
	    int bgt_id = 0;
	    
	    //Update bucket in the document
	    c.update((DATABASE_NAME.COLLECTION_NAME,QUERY(),BSON("bgt_id"<<bgt_id),true);
	    
            // query HMBL for socket list
            std::vector<Node *> SocketList = Map.get_Clients(location[0],location[1],radius);// need to pass in cli_addr, location, and radius

            // pass message to undecided server logic class that client will fill in
            // sadly this might be unavoidable
            // :-(
		
	    

	    //Create  structure
	    s_SUTMessage pipeStruct;
	    pipeStruct.message = completeMessage;
	    pipeStruct.SocketList = SocketList;
	    
            // pipe updates to send updates thread
            write(pipe, pipeStruct, sizeof(pipeStruct));
            
            
            //No Use now just for analytics
            mongo::BSONObjBuilder builder;
	    builder.appendElements(CompleteMessage);
	    builder.append("Socket",cli_addr);
	    BSONobj AnalyticsMessage = builder.obj();

        }

	}
}

uint16_t getNewPort()
{
  uint16_t portNo;
  struct sockaddr_in sa;
	
   Socket socket(0); // create a socket object
    socket.init(); // initialize/open the socket
    
    //get the dynamically generated port number
    getsockname(sock,(struct sockaddr*) &sa, &channellen);
    portNo = ntohs(sa.sin_port);	
    
    return portNo;
}
