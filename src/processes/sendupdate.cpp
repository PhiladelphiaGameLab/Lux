#include "sendupdate.h"
#include <arpa/inet.h>

#define DEBUG(x) do { if(true){ std::cout <<"[" << __TIME__ << " : " << __FILE__ << " : "<< __LINE__ << "]" << x << std::endl; } } while (0) 

using namespace mongo;
using namespace std;
using namespace socketlibrary;

void *SendUpdate::spawn(void*  param_in) {
	DEBUG("Send Updates Spawned");
	struct s_sut_params_in *params_in;
	params_in = (struct s_sut_params_in*)param_in;

        DEBUG("Opening battleground_sut pipe...");
	int FIFO = open(params_in->pipe_r, O_RDONLY);
	DEBUG("Opened battleground_sut pipe on sut side");
	//pipe(params_in->fd);
	
	DEBUG("Opening sut_db pipe...");
//	int FIFO2 = open(params_in->pipe_w, O_WRONLY);
	DEBUG("Opened sut_db pipe");
	
        LuxSocket socket;

	s_SUTMessage piped;
	

        //opening pipe for SUT_DB
	//int sut_db = open(params_in->pipe_w, O_WRONLY);
	//cout<<"Opened sut_db pipe on sut side"<<std::endl;

	// read parameters from BGTDoc
	// have BGTSpawner pass in the BGT_id -- add to s_sut_params_in struct
	
	while(true) {
	try{		
	DEBUG("Begining loop....");
	DEBUG("Reading from pipe....");
	read(FIFO, &piped, sizeof(s_SUTMessage));
	DEBUG("Pipe  has been read");
	DEBUG("Size of piped message : "<<sizeof(piped));
		//UDPSocket *test;
		vector<Node<sockaddr_in>*> clients = piped.SocketList;
	        DEBUG("Send Update size of list: "<< clients.size());
		if(clients.size() > 0){ 
			DEBUG("Client recieved : " << clients[0]);
	//		if(!(clients[0]))
                	for (vector<Node<sockaddr_in>*>::iterator client = clients.begin(); client != clients.end(); client++) {
                        	DEBUG("Entering For loop");

			//	DEBUG("locking....");
				//pthread_mutex_lock(&((*client)->Lock));
			//	DEBUG("Finished locking");	
      	 			
			// Temp ignore if seg fault
			DEBUG("Testing if *client is empty....");
			if((*client)){ 
			DEBUG("*client is not empty");
				try{
					DEBUG("Message sending...");
					if((&(*client)) && (&(*client)->sock) && (&(*client)->sock) != NULL){// && ((*client)->sock).sin_port != NULL){
						DEBUG("(*client) is" << (*client));
						//DEBUG("(*client)->sock is " << ((*client)->sock).sin_port);
						//DEBUG("Client Recieved with port address and ip" << inet_ntoa(((*client)->sock).sin_addr) << " : " << ntohs(((*client)->sock).sin_port));     
						socket.send(piped.message, &((*client)->sock)); //&cli_addr);
						DEBUG("message sent is" << piped.message.toString());
						DEBUG("Client Message sent to send socket");
					}else{
						DEBUG("Client socket == null Empty");
					}
					DEBUG("Message Sent");
				}catch(exception& e){
					DEBUG("Socket Failed to send!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
					DEBUG("error: " << e.what());
				}
			}else{
				DEBUG("*CLIENT IS NULL ON THIS RUN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			}
    			//	DEBUG("Unlocking....");
				//pthread_mutex_unlock(&((*client)->Lock));
			//	DEBUG("Unlocked");

	    			DEBUG("Exiting 'for' loop of sendupdate ");
			}
		}else{ 
			DEBUG("CLIENTS[0] IS NULL!!!"); 
		}		
        }catch(exception& e){
                cout << e.what() << endl;
        }
	
	//write(FIFO2, &piped.message, sizeof(BSONObj));
	}

    DEBUG("Exiting the send upddate thread");
}

