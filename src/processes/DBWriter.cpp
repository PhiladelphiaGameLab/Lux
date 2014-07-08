#include "DBWriter.h"

#define DEBUG(x) do { if(true){ std::cout <<"[" << __TIME__ << " : " << __FILE__ << " : "<< __LINE__ << "]" << x << std::endl; } } while (0) 
using namespace mongo;
using namespace std;


void *DBWriter::spawn(void* param_in){ // dbWriter thread
	DEBUG("Starting DB Writer");
	DEBUG("Converting Struct...");    
	struct s_dbWriter_params_in *params_in;
   	params_in = (struct s_dbWriter_params_in*)param_in;
	DEBUG("Converted Struct");
    
	DEBUG("Connecting to database...");
	DBClientConnection c;
	c.connect("localhost");
	DEBUG("Connected to database");

	DEBUG("Opening Pipe...");
	int FIFO = open(params_in->pipe_r, O_RDONLY);
	DEBUG("Pipe Opened");
    
	BSONObj msg;

    	while(1){
        	DEBUG("Entering DBWriter Loop");
		// get message
		DEBUG("Waiting to read pipe...");
        	read(FIFO, &msg, sizeof(BSONObj));
		DEBUG("Recieved Message on pipe");
	
		DEBUG("Checking that message is not empty");	
		if(!msg.isEmpty()){
			DEBUG("Inserting Message to Mongo....");
		//	c.update(DATABASE_NAME, BSON("_id" << msg["_id"]), msg);
				
		//	c.update(DATABASE_NAME, QUERY("_id"<<msg["_id"]),msg);//BSON("$set"<<BSON(msg)));
			DEBUG("Inerted to Mongo");
		}
		DEBUG("Checked that message is not empty");
    	}
    
}
