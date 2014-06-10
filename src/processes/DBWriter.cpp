#include "DBWriter.h"

using namespace mongo;
using namespace std;


void DBWriter::spawn(struct s_dbWriter_params_in params_in){ // dbWriter thread

    DBClientConnection c;
    c.connect("localhost");

    int FIFO = open(params_in.pipe_r, O_RDONLY);

    BSONObj msg;

    while(1){
        // get message
        read(FIFO, &msg, sizeof(BSONObj));
	if(!msg.isEmpty()){
		c.update(DATABASE_NAME, BSON("_id" << msg["_id"]), msg);	
		// last parameter sets upsert to true
		// c.update(DATABASE_NAME, QUERY ("bgt_id" << BGTDoc.bgt_id), msg, true);
	}
    }
    
}
