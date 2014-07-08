#include "battleground.h"
#define DEBUG(x) do { if(true){ std::cout <<"[" << __TIME__ << " : " << __FILE__ << " : "<< __LINE__ << "]" << x << std::endl; } } while (0) 

using namespace mongo;
using namespace std;
using namespace socketlibrary;

template class HMBL<sockaddr_in>;


void *BattleGround::spawn(void* param){
	DEBUG("BGT ID " << pthread_self());      

	// convert parameters
	DEBUG("Converting params...");
	struct s_bgt_params_in *param_in;
	param_in = (struct s_bgt_params_in*)param;
	DEBUG("Converted params");


 	//connect to the database
    	DEBUG("Connecting to Database...");
	DBClientConnection c;
    	c.connect("localhost");
	DEBUG("Connected to Database");
	
	// opening socket
	DEBUG("Opening Socket...");
	LuxSocket socket(3005);
	DEBUG("Opned Socket");
       
	// open the pipe
	DEBUG("Opening Pipe...");
	int pipes = open(param_in->pipe_w, O_WRONLY); // open  the pipe for writing
	DEBUG("Opened Pipe");

	// Created HMBL
	DEBUG("Creating HMBL...");
	HMBL<sockaddr_in> Map(100,100,5,5, param_in->pipe_hmbl);
	DEBUG("Created HMBL");


	//These 2 lines have to be uncommented to update the port in MongoDB
	// uint16_t portNo = getNewPort();
	// uint16_t socket.getPort();
	// c.update(DATABASE_NAME, BSON("port"<<0),portNo);

	while(1){
		sockaddr_in cli_addr;
		bool id_present = false;

		// accept clients, who will send in their updatei
		DEBUG("Waiting for clients to connect...");
		BSONObj message = socket.receive(&cli_addr);
        	DEBUG("Client Recieved");

		// get accessToken from BSONObj message
        	string accessToken = message["sender"]["accessToken"].toString(false); // this should be as easy as this- but might not be.
        	// get EUID from BSONObj message
        	string EUID = message["sender"]["EUID"].String();
		string tempid = message["tempid"].String();
		// authenticate message
		DEBUG("Authenticating Access Token and EUID..." << accessToken << " , " << EUID);
        	if(Authenticate::authenticateAccessToken(accessToken, EUID)){
        	DEBUG("Authenticated Access Token and EUID");
        		// getting system time
			DEBUG("Getting System Time...");	
	    		const long double sysTime = time(0);
	    		const long double timestamp = sysTime*1000;
	    		string currentTime= to_string(timestamp);
	    		DEBUG("Got System Time: " << currentTime);

	    		//Strip Access Token
	    		DEBUG("Stripping token...");
	    		BSONElement strippedEUIDMessage = message["sender"]["EUID"];
		
	    		//Strip message header
	    		BSONElement strippedObjectMessage = message["object"];
	   
			// Building Stripped Message
			DEBUG("Stripped token");
			BSONObjBuilder builder;
			// use: builder.genOID().append(....
	    		builder.append(strippedEUIDMessage);
			DEBUG("Appending EUID");
	    		builder.append(strippedObjectMessage);
			DEBUG("Appending Object");
	    		builder.append("time",currentTime);
			DEBUG("Appending Time");
			builder.append("tempid", tempid);
			DEBUG("Appending Tempid");
	   	 	BSONObj completeMessage = builder.obj();
			DEBUG("Finished Building Message");
			// Adding Document to Db is none Exists
	   		string id;
	   		id = completeMessage["_id"].toString();//toString(false);
			DEBUG("_id take 1 (" << "): " << id);
			DEBUG("id: " << id << " Complete Message "<< completeMessage.toString(true));	   
			//DEBUG("Query result: "<< !c.findOne(DATABASE_NAME,QUERY("_id"<<OID(id))).isEmpty());
			
			auto_ptr<DBClientCursor> cursor;
			DEBUG("Type of id" << typeid(id).name());			
			if(id.compare("EOO")!=0){
				id = completeMessage["_id"].OID().toString();
				DEBUG("Trying to get cursor");
				cursor = c.query(DATABASE_NAME,QUERY("_id"<<OID(id)));
				DEBUG("Obtained cursor");
			}else{
	      	        	DEBUG("Inserting Document...");
				c.insert(DATABASE_NAME,completeMessage);
				DEBUG("Inserted Document");

				auto_ptr<DBClientCursor> cursor2 = c.query(DATABASE_NAME,QUERY("tempid"<<tempid));
				if(cursor2->more()){
					BSONObj objectFromTemp  = cursor2->next();
					id = objectFromTemp["_id"].OID().toString();	
					DEBUG("Inside cursor loop value od _id is"<<id);
				}else{
					id = completeMessage["_id"].toString();
				}
			}
			
			DEBUG("_id take 2 (" << "fsdfasdfds" << "): " << id);
			DEBUG("Finished Checking if the document needed to be added");
			
			// Getting Document Location
			// get location from message
			DEBUG("Getting Object Location...");
			int locationX;
			locationX = atoi(message["object"]["location"]["x"].String().c_str());
			int locationY;
			locationY  = atoi(message["object"]["location"]["y"].String().c_str());
			int radius;
			radius  = atoi(message["object"]["radius"].String().c_str());
			std::cout<<"Location X:"<<locationX<<" Location Y:"<<locationY<<" radius:"<<radius<<std::endl;
			DEBUG("Got Object Location...");



			// Getting New Bucket Id
			DEBUG("Getting Bucket Number");
	    		int bucket_id = Map.findBucket(locationX,locationY);
	    		DEBUG("Got Bucket Number: " << bucket_id);

	    		DEBUG("Updating document bucket...");	
	     		c.update(DATABASE_NAME,QUERY("_id" << OID(id)), BSON("$set" << BSON("bucketID" << std::to_string(bucket_id) << "tempid" << 0))); // << "$set" << "tempid" << "null"));
	    		DEBUG("Updated Document Bucket");

			// Checking if the message is a client doci
			DEBUG("Checking for client Doc");
			string EU_DOC;
	    		EU_DOC = completeMessage["object"]["EU_DOC"].String();

	    		if(EU_DOC.compare("") != 0 && EU_DOC.compare("true")==0){			
				// updating map location
				DEBUG("Updating Map....");
				DEBUG("EUID : " << EUID);
	        		Map.update(cli_addr,stoi(EUID),locationX,locationY,radius);
	     			DEBUG("Map Updated...");
	     		}
			DEBUG("Checked for client Doc");


			// query HMBL for socket list
            		DEBUG("Getting Socket List....");
			vector<Node<sockaddr_in>*> SocketList = Map.get_clients(locationX,locationY,radius);// need to pass in cli_addr, location, and radius
			DEBUG("Got Socket List");
            		// pass message to undecided server logic class that client will fill in
           		// sadly this might be unavoidable
            		// :-(
		        
			if(!SocketList.empty()){
				DEBUG("Socket List: " << SocketList[0]);
	    		}else{
				DEBUG("Socket List is empty");
			}
			//Create  structure
	    		DEBUG("Creating Struct...");
			s_SUTMessage pipeStruct;
	    		pipeStruct.message = completeMessage;
	    		pipeStruct.SocketList = SocketList;
	    		DEBUG("Created Struct");
	    
			// Write to Pipe
			DEBUG("Writing To pipe.... "<< sizeof(pipeStruct));
			DEBUG("Message: " << pipeStruct.message);
            		write(pipes, &pipeStruct, sizeof(pipeStruct));
			DEBUG("Wrote to Pipe");
		
			//sleep(5);	
			/*
             		std::cout<<"Creating BSONobj for Analytics"<<std::endl;
            		
			// No Use now just for analytics
            		BSONObjBuilder build;
	    		build.appendElements(completeMessage);
	   		// build.append("Socket",cli_addr);
	    		BSONObj AnalyticsMessage;
	    		AnalyticsMessage = build.obj();
			*/

			//}
		}

	}
}
 
