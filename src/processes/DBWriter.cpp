#include "DBWriter.h"

void DBWriter::spawn(struct pipe params_in){ // dbWriter thread
    mongo::DBClientConnection c;
    c.connect("localhost");

    int FIFO = open(params_in.pipe, O_RDONLY);

    while(1){
        // get message
        read(FIFO, msg, sizeof(BSONObj));

        // c.update(DATABASE_NAME, QUERY(), BSON("$inc"<<BSON("a"<<2)), {upsert:true});
        
        db.c.update(
            { /*leaving empty returns all documents in this collection*/ },
            //{ $set:{'title':'New MongoDB Tutorial'} },
            { msg },
            { upsert:true }
        );


    }
    
}
