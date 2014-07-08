#include "SendNewRelevant.h"
#include <arpa/inet.h>

#define DEBUG(x) do { if(true){ std::cout <<"[" << __TIME__ << " : " << __FILE__ << " : "<< __LINE__ << "]" << x << std::endl; } } while (0) 
 
void *SendNewRelevant::spawn(void*  param_in){
    std::cout << "HELP! SNR : 1 " << std::endl;
    DEBUG("Connecting to Database...");
    mongo::DBClientConnection c;
    c.connect("localhost");
    DEBUG("Connected to Database.");

     struct s_snr_params_in *params_in;
     params_in = (struct s_snr_params_in*)param_in;


        LuxSocket socket;
	DEBUG("opening pipe in SNR");  
	int snr_pipe = open(params_in->pipe_r, O_RDONLY);
	DEBUG("opened pipe in SNR");
        // pipe(params_in->fd);

//	int ss_fd[2];
//      memcpy(ss_fd,params_in->fd,sizeof(params_in->fd));


	struct s_SNRMessage piped;

    while(1){
	std::cout << "HELP! SNR : 2 " << std::endl;
        // read socket
        // read(ss_fd[0], &piped, sizeof(s_SNRMessage));
	DEBUG("Attempting to read pipe...");
        read(snr_pipe, &piped, sizeof(s_SNRMessage));
	DEBUG("Pipe read.");

	//DEBUG("Client Recieved with port address and ip" <<ntohs(((*clientVector)->sock).sin_port) <<","<<inet_ntoa(((*clientVector)->sock).sin_addr))
	DEBUG("Recieved client socket : " <<inet_ntoa(piped.socket.sin_addr) << " : " << ntohs(piped.socket.sin_port) );
        // read documents from mongo
       // DEBUG("Bucket element 1 : " << piped.newBucketList);
        DEBUG("Bucket List size : "<< piped.newBucketList.size());
        for (vector<int>::iterator bucket = piped.newBucketList.begin(); bucket != piped.newBucketList.end(); bucket++){
            // query the database
            DEBUG("Querying " << *bucket << "...");
	    //DEBUG(c.query(DATABASE_NAME, QUERY("bucketID" << (*bucket))->next().toString(true)));
            auto_ptr<DBClientCursor> cursor = c.query(DATABASE_NAME, QUERY("bucketID" << (*bucket)));
            DEBUG("Database Queried in bucket " << *bucket << " .");

            // iterate elements from the buckets
            DEBUG("Iterating through Objects in "<< *bucket << "...");
	    while (cursor->more()){
		DEBUG("About to send object");
                // strip sender access token & such
		BSONObj obj = cursor->next();
                // send both client and message to the socket Class
                socket.send(obj,&piped.socket);
		DEBUG("Sent Object");
            }
        }
    }
}
