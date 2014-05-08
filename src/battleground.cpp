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

    // create pipe to send updates on
    char * sendUpdates = "/temp/sendUpdates";
    mkfifo(sendUpdates, 0666);
    int SP = open(sendUpdates, O_WRONLY);
    pthread_create(&BGTID, NULL, (void *) &SendUpdate::createSUT, (void *) &BGTparam); // spawn sendUpdates thread

    Socket recieveSocket;
	recieveSocket.init();

	while(1){
	    // accept clients, who will send in their "User Token"
		BSONObj message = recieveSocket.recieve(&cli_addr);

        // get accessToken from BSONObj message
        std::string accessToken = ;
        // get EUID from BSONObj message
        std::string EUID = ;


        // authenticate message
        if(Authenticate::authenticateAccessToken(accessToken, EUID)){
            // get location from message
            int[3] location = ;
            int radius = ;

            // query HMBL for socket list
            std::list<struct sockaddr_in> SocketList = ;

            // pipe updates to send updates thread
            write(SP, message, sizeof(message));

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
