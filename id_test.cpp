#include "id_test.h"
#include <arpa/inet.h>

#define DEBUG(x) do { if(true){ std::cout <<"[" << __TIME__ << " : " << __FILE__ << " : "<< __LINE__ << "]" << x << std::endl; } } while (0) 

using namespace mongo;
using namespace std;


int main(){
	DEBUG("BGT ID " << pthread_self());      

	// convert parameters

 	//connect to the database
    	DEBUG("Connecting to Database...");
	DBClientConnection c;
    	c.connect("localhost");
	DEBUG("Connected to Database");
	

	//These 2 lines have to be uncommented to update the port in MongoDB
	// uint16_t portNo = getNewPort();
	// uint16_t socket.getPort();
	// c.update(DATABASE_NAME, BSON("port"<<0),portNo);

		sockaddr_in cli_addr;
		bool id_present = false;

			//Checing if EUID exists

			string id = "53c051fa7c248fae688f6c7e";
	    		DEBUG("Updating document bucket...");	
	     	//	BSONObj loc = BSON("X"<<15<<"y"<<10<<"z"<<5);
			//c.update(DATABASE_NAME,QUERY("_id" << OID(id)), BSON("$set" << BSON("bucketID" << 100  << "tempid" << 0 <<"radius" <<6 <<"EUID"<<234)),BSON("$push"<<BSON("location"<<BSON("x":55)))); // << "$set" << "tempid" << "null"));
			BSONObjBuilder bOb;

//			 c.update(DATABASE_NAME,QUERY("_id" << OID(id)), BSON("$set" << BSON("bucketID" << 100  << "tempid" << 0 <<"radius" <<6 <<"EUID"<<234<<BSONArray(BSON("Location"<<BSONArray(BSON("x"<<7<<"y"<<8<<"z"<<9)))))));


			BSONObj testobj = BSON("bucketID" << 100  << "tempid" << 0 <<"radius" <<6 <<"EUID"<<234<<"Location"<<BSON("x"<<10<<"y"<<15<<"z"<<20)<<"object"<<BSON("animation"<<"none"<<"sound"<<"cool"<<"model"<<"chair"));
			 c.update(DATABASE_NAME,QUERY("_id" << OID(id)), BSON("$set" << testobj));

	    		DEBUG("Updated Document Bucket");
			
			DEBUG("Pulling message from db....");
			auto_ptr<DBClientCursor> cursor2 = c.query(DATABASE_NAME,QUERY("_id"<<OID(id)));
			if(cursor2->more())
			{
			 DEBUG("Can get BSONObj");
			 //DEBUG("whats der?",cursor2->next().toString(true));
			 DEBUG("Got the message");
			}
	return 0;
	}
 
