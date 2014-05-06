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

struct pipe{
    int pipe;
};

static void SendUpdate::createSUT(int sendFIFO, HashMapBasedLocation* HMBL){
    // static function so pthread can launch an instance of the BGT class
    SendUpdate SendUpdate::SendUpdate(int sendFIFO, HashMapBasedLocation* HMBL);
}

static void SendUpdate::dbWriter(struct pipe dbP){ // dbWriter thread
    mongo::DBClientConnection c;
    c.connect("localhost");

    int FIFO = open(dbP.pipe, O_RDONLY);

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


static void SendUpdate::sendNewRelevant(struct pipe newRevPipe){

    mongo::DBClientConnection c;
    c.connect("localhost");

    Socket sendSocket;
	sendSocket.init();

	int FIFO = open(newRevPipe.pipe, O_RDONLY);
	struct newConnectionInfo piped;

    while(1){
        // read socket
        read(FIFO, piped, MAX_BUF);
        // read documents from mongo
        for (std::list<int bucketID>::iterator bucket = piped->BucketList.begin(); bucket != piped->BucketList.end(); bucket++){
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


SendUpdate::SendUpdate(struct sendUpdateArgs SUTA){

    mkfifo("/temp/dbWriter", 0666);
    int DBW = open(dbWriter, O_WRONLY);
    struct pipe dbP;
    dbP.pipe = DBW;
    pthread_create(&BGTID, NULL, (void *) &SendUpdate::dbWriter, (void *) &dbP); // spawn dbWriter thread

    // establish new sending port
    Socket sendSocket;
	sendSocket.init();

	int readPipe = open(SUTA.pipe, O_RDONLY);

	struct sendUpdates msg;
	while(1){
		read(readPipe, msg, MAX_BUF);
        // break msg into sockets & object
        // send to socket list via socket class
        sendSocket.send(msg.SocketList, msg.message);
        // write message to database thread
        if(SUTA.writeToDb > 0){
            write(dbWriter, msg.message, sizeof(msg.message));
        }
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
