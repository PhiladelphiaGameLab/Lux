#include "sendupdate.h"

using namespace mongo;
using namespace std;
using namespace socketlibrary;

void *SendUpdate::spawn(void*  param_in) {
	std::cout << "HELP! SendUpdate : 1 " << std::endl;
	struct s_sut_params_in *params_in;
	params_in = (struct s_sut_params_in*)param_in;

	int FIFO = open(params_in->pipe_r, O_RDONLY);

	LuxSocket socket;

	s_SUTMessage piped;
	
	// read parameters from BGTDoc
	// have BGTSpawner pass in the BGT_id -- add to s_sut_params_in struct
	//std::vector<Node<sockaddr_in>*> clients = HMBL<sockaddr_in>::get_clients(int xloc, int yloc, int rad);
	
	while(true) {
		
	std::cout << "HELP! Send update : 2 " << std::endl;
	read(FIFO, &piped, sizeof(s_SUTMessage));
	cout<<"Size of s_SUTMessage : "<<sizeof(s_SUTMessage)<<endl;
	std::cout<<"Recieved in sendupdate messsage"<<piped.message.toString() <<std::endl;

//	if (piped.SocketList == NULL)
 //	{
//		std::cout<<<"Error piped.socketList is Null"<<std::endl;
//	}
//	else
//	{

	//	vector<Node<sockaddr_in>*> clis = piped.SocketList;
	//	for(vector<Node<sockaddr_in>*>::iterator cli = clis.begin();cli!=clis.end();cli++)
	//	{
	//		std::cout<<"Sending Messagefrom the 1st for loop"<<std::endl;
	//	}
//	}
		vector<Node<sockaddr_in>*> clients = piped.SocketList;
	std::cout<< "Send Update size of list: "<<piped.SocketList.size()<< std::endl;
		for (vector<Node<sockaddr_in>*>::iterator clientVector = clients.begin();clientVector != clients.end(); clientVector++) {
			cout<<"Entering 'for' loop of sendupdate " <<clients[0] << endl;
			pthread_mutex_lock((*clientVector)->Lock);
			cout<<"After Locking"<<endl;
			sockaddr_in cli_addr = (*clientVector)->sock;
			std::cout << "Sending message "<< std::endl;
			socket.send(piped.message, &cli_addr);
    			pthread_mutex_unlock((*clientVector)->Lock);
		}
	}

    std::cout<< "Exiting the send upddate thread" <<std::endl;
	}

