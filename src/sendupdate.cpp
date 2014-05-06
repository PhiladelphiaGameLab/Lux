/*
 * This code was orginally written and developed by the Lux Backend Team
 * at Philadelphia Game Lab:
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * - Jake
 */

#include "sendupdate.h"
static void SendUpdate::createSUT(int sendFIFO, HashMapBasedLocation* HMBL){
    // static function so pthread can launch an instance of the BGT class
    SendUpdate SendUpdate::SendUpdate(int sendFIFO, HashMapBasedLocation* HMBL);
}

static void SendUpdate::dbWriter(){ // dbWriter thread
    // get message
    // write to database
}

SendUpdate::SendUpdate(int sendFIFO, HashMapBasedLocation* HMBL){

    char * dbWriter = "/temp/dbWriter";
    mkfifo(dbWriter, 0666);
    int SP = open(dbWriter, O_WRONLY);
    pthread_create(&BGTID, NULL, (void *) &SendUpdate::dbWriter, void); // spawn dbWriter thread

    // establish new sending port
    Socket sendSocket;
	sendSocket.init();

	FIFO = open(sendFIFO, O_RDONLY);
	struct sendUpdates msg;
	while(1){
		read(FIFO, msg, MAX_BUF);
        // break msg into sockets & object
        // send to socket list via socket class
        sendSocket.send(msg.SocketList, msg.message);
        // write message to database thread
        write(dbWriter, msg.message, sizeof(msg.message));
}

SendUpdate::~SendUpdate(){//dtor
}

SendUpdate::SendUpdate(const SendUpdate& other){//copy ctor
}

SendUpdate& SendUpdate::operator=(const SendUpdate& rhs){
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
