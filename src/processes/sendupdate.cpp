#include "sendupdate.h"

void SendUpdate::spawn(struct sendUpdateArgs params_in){
    if(params_in.writeToDb > 0){
        mkfifo("/temp/dbWriter", 0666);
        int DBW = open(dbWriter, O_WRONLY);
        struct pipe dbP;
        dbP.pipe = DBW;
        pthread_create(&BGTID, NULL, (void *) &SendUpdate::dbWriter, (void *) &dbP); // spawn dbWriter thread
    }

    // establish new sending port
    Socket sendSocket;
	sendSocket.init();

	int FIFO = open(params_in.pipe, O_RDONLY);

	struct sendUpdates piped;
	while(1){
		read(FIFO, piped, MAX_BUF);
        // break piped into sockets & object
        // send to socket list via socket class
        sendSocket.send(piped.SocketList, piped.message);
        // write message to database thread
        if(params_in.writeToDb > 0){
            write(dbWriter, piped.message, sizeof(piped.message));
        }
    }
}
