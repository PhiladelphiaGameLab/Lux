#include "SendNewRelevant.h"



void SendNewRelevant::spawn(void*  param_in){

    mongo::DBClientConnection c;
    c.connect("localhost");

     struct s_snr_params_in *params_in;
     params_in = (stuct s_snr_params_in*)param_in;


    Socket sendSocket;
	sendSocket.init();

	int FIFO = open(params_in->pipe_r, O_RDONLY);
	struct newConnectionInfo piped;

    while(1){
        // read socket
        read(FIFO, piped, MAX_BUF);
        // read documents from mongo
        for (std::list<int>::iterator bucket = piped.BucketList.begin(); bucket != piped.BucketList.end(); bucket++){
            // query the database
            auto_ptr<DBClientCursor> cursor = c.query(DATABASE_NAME, QUERY("bucketID" << bucket) ));
            // iterate elements from the buckets
            while (cursor->more()){
                // strip sender access token & such

                // send both client and message to the socket Class
                sendSocket.send(piped.socket, cursor->next().jsonString());
            }
        }
    }
}
