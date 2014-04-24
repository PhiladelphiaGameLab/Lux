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

#include "battleground.h"

static void BattleGround::createBGT(){
    // static function so pthread can launch an instance of the BGT class
    BattleGroundThread BGT(BGTParam.port, BGTParam.DBConnection, BGTParam.BGTIdentity......... );
}

void BattleGround::newConnectionsThread(){
// need to handle new connections to the BGT
}

BattleGround::BattleGround(int[6] location, char[20] name, int id, int port, HashMapBasedLocation* HMBL, struct DBC DBConnection, int AP){
        // DO SOMETHING DOCUMENT DATABASE RELATED

    char * shout = "/temp/shout";
    mkfifo(shout, 0666);
    int SP = open(shout, O_WRONLY);
    pthread_create(&BGTID, NULL, (void *) &createSUT, (void *) &BGTparam); // spawn BGT

    char * broadcast = "/temp/broadcast";
    mkfifo(broadcast, 0666);
    int BCP  = open(broadcast, O_WRONLY);
    pthread_create(&BGTID, NULL, (void *) &createSUT, (void *) &BGTparam); // spawn BGT

    char * newConnection = "/temp/newConnection";
    mkfifo(newConnection, 0666);
    int NCP = open(newConnection, O_WRONLY);
    pthread_create(&BGTID, NULL, (void *) &newConnectionsThread, (void *) &BGTparam); // spawn BGT

    Socket recieveSocket;
	recieveSocket.init();

	while(1){
	    // accept clients, who will send in their "User Token"
		socket.recieve(&cli_addr);
		// this will be how we read the "User Token"
		char clientMessage[256] = socket.GetMessage();
		JSObject message = JSON.parse(clientMessage);

		 // if the client checks out
		if(verifyClient(message.clientID, message.Token) > 0){
            int updated = HMBL.updateLocation(cli_addr, message.client_location);
            if(updated > 0){
                if(message.type == "broadcast"){
                    write(BCP, JSON.stringify(message), sizeof(message));
                }else if(message.type == "shout"){
                    write(SP, JSON.stringify(message), sizeof(message));
                }
                write(AP, JSON.stringify(message), sizeof(message)); // send to analyitics

                // UPDATE IN DOCUMENT DB

            }else if(updated == -1){ // not in correct thread
                socket.send(&cli_addr, "{error: \"This is not the thread you are looking for\"}");
            }else if(updated == 0){ // new user
                write(NCP, cli_addr, sizeof(cli_addr));
            }

		}else{
		    // Client Didn't Check out.
            socket.send(&cli_addr, "{error: \"Error Verifying Identity\"}");
		}

	}
}

BattleGround::~BattleGround()
{
    //dtor
}

BattleGround::BattleGround(const BattleGround& other)
{
    //copy ctor
}

BattleGround& BattleGround::operator=(const BattleGround& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
