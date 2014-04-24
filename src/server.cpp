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

#include <iostream>
#include <sys/types.h> // definitions for system types
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> // needed for internet domain routing
#include <string.h>
#include <netdb.h>
#include <stdio.h> // standard libraries
#include <list>
#include <queue>
#include "socket.h"

#define MESSAGE_SIZE 256

int verifyClient(std::string clientID, std::string token){
    return 1;
}
int main(int argc, char *argv[]){

    // this is where failures and recovery should be handled
    // does not need to be a new class, just wraps around this class and monitors that the server is running

    // also should handle when pushing out an update and with minimal interuptions

    MainMessagePassingServer MMPS(3000, "./config");
    MMPS.createThreads(1);
    MMPS.start();
	return 0; // should be unreachable
}
