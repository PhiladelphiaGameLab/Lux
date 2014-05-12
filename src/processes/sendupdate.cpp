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
