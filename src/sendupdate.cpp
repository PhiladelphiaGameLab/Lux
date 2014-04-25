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


SendUpdate::SendUpdate(int sendFIFO, HashMapBasedLocation* HMBL){
    // establish new sending port
    Socket sendSocket;
	sendSocket.init();

	// buffer for messages
    json_t msg;
	FIFO = open(sendFIFO, O_RDONLY);
	while(1){
		read(FIFO, msg, MAX_BUF);
        // read location from header
        const char *message_type = json_string_value(json_object_get(msg, "type"));
        // Test if broadcast or shout, if shout, check location and
        std::list<struct sockaddr_in> SocketList;
        if(message_type == "broadcast"){
            SocketList = HMBL.getSockets();
        }else if(message_type == "shout"){
            // loop through all the values at location and put them in an array
            json_t location = json_object_get(msg, "location");
            int message_location[json_array_size(location)];
            for(i = 0; i < json_array_size(location); i++){
                message_location[i] = json_array_get(location, i);
            }
            SocketList = HMBL.getSockets(message_location);
        }

        // Strip header from message
        json_t sendingMessage = json_object_get(msg, "object");

		// this is the send command
		sendSocket.send(SocketList, sendingMessage);
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
