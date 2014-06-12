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

using namespace mongo;
using namespace std;
using namespace socketlibrary;

template class HMBL<sockaddr_in>;

void *BattleGround::spawn(void* param){
	struct s_bgt_params_in *param_in;
	param_in = (struct s_bgt_params_in*)param;

 	//connect to the database
    	DBClientConnection c;
    	c.connect("localhost");

    // this pipe stuff should be right:

    // create pipe to send updates on
	LuxSocket socket(0);
        int pipe = open(param_in->pipe_w, O_WRONLY); // open  the pipe for writing


     // construct a HMBL
     //locbasedhashmap HMBL;
     //HMBL<sockaddr_in> Map(mapSizeX,mapSizeY,threadSizeX,threadSizeY, param_in.pipe_hmbl);
     HMBL<sockaddr_in> Map(100,100,5,5, param_in->pipe_hmbl);


     //These 2 lines have to be uncommented to update the port in MongoDB
     // uint16_t portNo = getNewPort();
     // uint16_t socket.getPort();
     // c.update(DATABASE_NAME, BSON("port"<<0),portNo);

    // will need to pass the socket that was opened back to the
    // spawn BGT so that it can use that later for redirection

	while(1){
	 sockaddr_in cli_addr;
	 // accept clients, who will send in their update
	BSONObj message = socket.receive(&cli_addr);

        // get accessToken from BSONObj message
        string accessToken = message["sender"]["accessToken"].String(); // this should be as easy as this- but might not be.
        // get EUID from BSONObj message
        string EUID = message["sender"]["EUID"].String();

	
		
	
        // authenticate message
        if(Authenticate::authenticateAccessToken(accessToken, EUID)){
        	
        	
            //timestamp (Current time of the system
	    
//	    char timestamp[20];
//	    time_t now = time(0);
	    const long double sysTime = time(0);
	    const long double timestamp = sysTime*1000;
            
	    string currentTime= to_string(timestamp);
	    

	    //Strip Access Token
	    BSONElement strippedEUIDMessage = message["sender"]["EUID"];
		
	    //Strip message header
	    BSONElement strippedObjectMessage = message["object"];
	    
	    BSONObjBuilder builder;
	    builder.append(strippedEUIDMessage);
	    builder.append(strippedObjectMessage);
	    builder.append("time",currentTime);
	    BSONObj completeMessage = builder.obj();
	
	
	   //get the id of the message
	   BSONElement id;
	   id = completeMessage["_id"];
	   
           //Check for id (Maybe cursor is needed)
           if(!c.findOne(DATABASE_NAME,QUERY("_id"<<id)).isEmpty())
           {
           	//insert into MongoDB
           	c.insert(DATABASE_NAME,completeMessage);
           }
        	
            // get location from message
            int locationX;
	    locationX = atoi(message["object"]["location"]["x"].String().c_str());
            int locationY;
	    locationY  = atoi(message["object"]["location"]["y"].String().c_str());
            int radius;
	    radius  = atoi(message["object"]["radius"].String().c_str());

	
	    //Get the values of EU_DOC and cli_obj_doc
	    string EU_DOC;
	    EU_DOC = completeMessage["object"]["EU_DOC"].String();

	    if(EU_DOC.compare("") != 0 && EU_DOC.compare("true")==0){
	        //Updte clients location in HMBL
	        Map.update(cli_addr,stoi(EUID),locationX,locationY,radius);
	    }
	     	
	     	
	    //get bucket ID
	    //int bgt_id = Map.getBucket(location[0],location[1],mapSizeX,mapSizeY,thredSizeX,threadSizeY);
	    int bgt_id = 0;
	    
	    //Update bucket in the document
	    c.update(DATABASE_NAME,BSON("_id"<<id),BSON("bgt_id"<< bgt_id));
	    
            // query HMBL for socket list
            vector<Node<sockaddr_in>*> SocketList = Map.get_clients(locationX,locationY,radius);// need to pass in cli_addr, location, and radius

            // pass message to undecided server logic class that client will fill in
            // sadly this might be unavoidable
            // :-(
		
	    

	    //Create  structure
	    s_SUTMessage pipeStruct;
	    pipeStruct.message = completeMessage;
	    pipeStruct.SocketList = SocketList;
	    
            // pipe updates to send updates thread
            write(pipe, &pipeStruct, sizeof(pipeStruct));
            
            
            //No Use now just for analytics
            BSONObjBuilder build;
	    build.appendElements(completeMessage);
	   // build.append("Socket",cli_addr);
	    BSONObj AnalyticsMessage;
	    AnalyticsMessage = build.obj();

        }

	}
}
 
