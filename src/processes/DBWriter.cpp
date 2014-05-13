#include "DBWriter.h"

void DBWriter::spawn(struct pipe params_in){ // dbWriter thread
    mongo::DBClientConnection c;
    c.connect("localhost");

    int FIFO = open(params_in.pipe, O_RDONLY);

    while(1){
        // get message
        read(FIFO, msg, MAX_BUF);
        // strip message header
                                // WHERE    // SET
                                // read _id     set all w/ upsert
        c.update(DATABASE_NAME, QUERY(),    BSON("$inc"<<BSON("a"<<2)), false, true);
        // write to database
    }
}
