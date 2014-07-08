#include "id_test.h"
#define DEBUG(x) do { if(true){ std::cout <<"[" << __TIME__ << " : " << __FILE__ << " : "<< __LINE__ << "]" << x << std::endl; } } while (0) 

using namespace mongo;
using namespace std;
//using namespace socketlibrary;


int main(){
//	DEBUG("BGT ID " << pthread_self());      

	// convert parameters


 	//connect to the database
    	DEBUG("Connecting to Database...");
	DBClientConnection c;
    	c.connect("localhost");
	DEBUG("Connected to Database");

                        string id;
                        id = "53bb4966f65afb8b3b5eb17c";//toString(false);
                        DEBUG("_id take 1 (""): " << id);
                       // DEBUG("id: " << id << " \n Complete Message "<< completeMessage.toString(true));
                       // DEBUG("Query result: "<< !c.findOne(DATABASE_NAME,QUERY("_id"<<id)).isEmpty());
                       DEBUG("Query result with findone: "<< c.findOne(DATABASE_NAME,Query().toString()));
                       auto_ptr<DBClientCursor> cursor = c.query(DATABASE_NAME,QUERY("_id"<<OID(id)));

			while (cursor->more())
			{
   			//DEBUG("Query result with cursor"<< cursor->next().toString());
			BSONObj x = cursor->next();
			DEBUG("Query result with cursor"<< x.toString());
			DEBUG("BSONobj extracted what is its id?"<<x["_id"].toString());
			}

                        BSONElement id2;
                        if(c.findOne(DATABASE_NAME,QUERY("_id"<<id)).isEmpty()){
                                DEBUG("Inserting Document...");
                               // c.insert(DATABASE_NAME,completeMessage);
                                DEBUG("Inserted Document");
                               // id = c.findOne(DATABASE_NAME,QUERY("tempid"<<tempid))["_id"].toString();
                               // id2 = c.findOne(DATABASE_NAME,QUERY("tempid"<<tempid))["_id"];
                               // DEBUG("_id take 2 (jghjddfhj): " << id.substr(5));
                        }
                        DEBUG("Finished Checking if the document needed to be added");




			// Getting New Bucket Id
			//DEBUG("Getting Bucket Number");
	    		//int bucket_id = Map.findBucket(locationX,locationY);
	    		//DEBUG("Got Bucket Number: " << bucket_id);



			BSONObjBuilder Quer;
			Quer.append("_id",id);
			BSONObj xx = c.findOne(DATABASE_NAME,Quer.obj());
			DEBUG("testing.... query:"<<xx.toString(true));			
			
	    		//Update bucket in the document
	    		DEBUG("Updating document bucket...");
			//DEBUG("EU_DOC from _id based Query 0: " << c.findOne(DATABASE_NAME,QUERY("_id" << id2.getField("_id")))["EU_DOC"]);
			//DEBUG("EU_DOC from _id based Query 1: " << c.findOne(DATABASE_NAME,QUERY("_id" << OID(id2.getField("_id"))))["EU_DOC"]);
			//DEBUG("EU_DOC from _id based Query 2: " << c.findOne(DATABASE_NAME,QUERY("_id" << OID(id)))["EU_DOC"]);
			//DEBUG("EU_DOC from _id based Query 3: " << id.substr(id.find("'"), id.find("'", id.find("'")+1)));
			//DEBUG("EU_DOC from _id based Query 4: " << c.findOne(DATABASE_NAME,QUERY("_id" << OID(id.substr(id.find("'"), id.find("'", id.find("'")+1)))))["EU_DOC"]);
	     		//c.update(DATABASE_NAME,QUERY("_id" << id), BSON("$set" << BSON("bucketID" << std::to_string(5)))); // << "$set" << "tempid" << "null"));
	     		

			 c.update(DATABASE_NAME,QUERY("_id" << OID(id)), BSON("$set" << BSON("bucketID" << 10 << "tempid" << 0))); 
			return 0;
}
