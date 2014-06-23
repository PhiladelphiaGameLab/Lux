#include "DBWriter.h"

using namespace mongo;
using namespace std;


void *DBWriter::spawn(void* param_in){ // dbWriter thread
    std::cout << "HELP! DBWriter : 1 " << endl;
    struct s_dbWriter_params_in *params_in;
    params_in = (struct s_dbWriter_params_in*)param_in;

    DBClientConnection c;
    c.connect("localhost");

    int FIFO = open(params_in->pipe_r, O_RDONLY);

    BSONObj msg;

    while(1){
 	 std::cout << "HELP! DBWriter : 2 " << std::endl;
        // get message
        read(FIFO, &msg, sizeof(BSONObj));
	if(!msg.isEmpty()){
		c.update(DATABASE_NAME, BSON("_id" << msg["_id"]), msg);	
		// last parameter sets upsert to true
		// c.update(DATABASE_NAME, QUERY ("bgt_id" << BGTDoc.bgt_id), msg, true);
	}
    }
    
}
