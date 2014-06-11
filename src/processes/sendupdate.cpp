#include "sendupdate.h"

using namespace mongo;
using namespace std;
using namespace socketlibrary;

void *SendUpdate::spawn(void*  param_in) {

	struct s_sut_params_in *params_in;
	params_in = (struct s_sut_params_in*)param_in;

	int FIFO = open(params_in->pipe_r, O_RDONLY);

	LuxSocket socket;

	s_SUTMessage piped;
	
	// read parameters from BGTDoc
	// have BGTSpawner pass in the BGT_id -- add to s_sut_params_in struct
	//std::vector<Node<sockaddr_in>*> clients = HMBL<sockaddr_in>::get_clients(int xloc, int yloc, int rad);
	
	while(true) {
		
		read(FIFO, &piped, sizeof(s_SUTMessage));
		vector<Node<sockaddr_in>*> clients = piped.SocketList;
		for (vector<Node<sockaddr_in>*>::iterator clientVector = clients.begin();clientVector != clients.end(); clientVector++) {
			pthread_mutex_lock((*clientVector)->Lock);
			sockaddr_in cli_addr = (*clientVector)->sock;
			socket.send(piped.message, &cli_addr);
    			pthread_mutex_unlock((*clientVector)->Lock);
		}}
	}

