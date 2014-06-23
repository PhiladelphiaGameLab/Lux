#include "SendNewRelevant.h"

void *SendNewRelevant::spawn(void*  param_in){
    std::cout << "HELP! SNR : 1 " << std::endl;
    mongo::DBClientConnection c;
    c.connect("localhost");

     struct s_snr_params_in *params_in;
     params_in = (struct s_snr_params_in*)param_in;


        LuxSocket socket;
        std::cout<<"opening pipe in SNR"<<std::endl;  
	int FIFO = open(params_in->pipe_r, O_RDONLY);
	struct s_SNRMessage piped;

    while(1){
	 std::cout << "HELP! SNR : 2 " << std::endl;
        // read socket
        read(FIFO, &piped, sizeof(s_SNRMessage));
        // read documents from mongo
        cout<<"piped.newBucketList.size : "<<piped.newBucketList.size()<<endl;
        for (vector<int>::iterator bucket = piped.newBucketList.begin(); bucket != piped.newBucketList.end(); bucket++){

		//cout<<"Entering SNR for loop"<<endl;
            // query the database
            auto_ptr<DBClientCursor> cursor = c.query(DATABASE_NAME, QUERY("bucketID" << (*bucket)));
            // iterate elements from the buckets
            while (cursor->more()){
                // strip sender access token & such
		BSONObj obj = cursor->next();
                // send both client and message to the socket Class
                socket.send(obj,&piped.socket);
            }
        }
    }
}
