#include "DBWriter.h"

void DBWriter::spawn(struct s_dbWriter_params_in params_in){ // dbWriter thread

    mongo::DBClientConnection c;
    c.connect("localhost");

    int FIFO = open(params_in.pipe, O_RDONLY);

    while(1){
        // get message
        read(FIFO, msg, sizeof(BSONObj));

        // last parameter sets upsert to true
        c.update(DATABASE_NAME, QUERY ("bgt_id" << BGTDoc.bgt_id), msg, true);

    }
    
}
