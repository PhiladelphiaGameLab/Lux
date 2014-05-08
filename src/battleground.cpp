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

void static BattleGround::battleGround(void *param){

    // this pipe stuff should be right:

    // create pipe to send updates on
    const char *pipeLocation = "/temp/pipe"; // needs to be unique somehow
    if(mkfifo(pipeLocation, 0666) == 0){ // creates the pipe in the file system
        int pipe = open(pipeLocation, O_WRONLY); // open  the pipe for writing

        // so should the idea of this pthread

        s_SUT param;
        param.pipeLocation = pipeLocation;

        pthread_create(&BGTID, NULL, (void *) &SendUpdate::createSUT, (void *) &param); // spawn sendUpdates thread

    }


    // construct a HMBL

    Socket socket; // create a socket object
	socket.init(); // initialize/open the socket

    // will need to pass the socket that was opened back to the
    // spawn BGT so that it can use that later for redirection

	while(1){
	    sockaddr_in cli_addr;
	    // accept clients, who will send in their "User Token"
		BSONObj message = socket.recieve(&cli_addr);

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
            std::list<struct sockaddr_in> SocketList = ;// need to pass in cli_addr

            // pipe updates to send updates thread
            write(pipe, message, sizeof(message));

        }

	}
}
