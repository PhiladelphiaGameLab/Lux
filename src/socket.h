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
#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <sys/types.h> // definitions for system types
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> // needed for internet domain routing
#include <string.h>
#include <netdb.h>
#include <stdio.h> // standard libraries

#define MESSAGE_SIZE 256

class Socket{
    public:
        /** Default constructor */
        Socket(int port);
        Socket();
        void init();
        void error(char* msg);
        BSONObj recieve(struct sockaddr_in* cli_addr);
        void send(struct sockaddr_in* cli_addr);
        void send(struct sockaddr_in* cli_addr, BSONObj BSMessage);
        void send(std::list<struct sockaddr_in> SocketList, BSONObj BSMessage);
        void send(struct sockaddr_in* cli_addr, std::string message);
        void Socket::send(struct sockaddr_in* cli_addr, char * message[]);
        int main();
        /** Default destructor */
        virtual ~Socket();
        /** Access port
         * \return The current value of port
         */
        unsigned short Getport() { return port; }
        /** Set port
         * \param val New value to set
         */
        void Setport(unsigned short val) { port = val; }
        /** Access sin_family
         * \return The current value of sin_family
         */
        short Getsin_family() { return sin_family; }
        /** Set sin_family
         * \param val New value to set
         */
        void Setsin_family(short val) { sin_family = val; }
        /** Access sin_port
         * \return The current value of sin_port
         */
        unsigned short Getsin_port() { return sin_port; }
        /** Set sin_port
         * \param val New value to set
         */
        void Setsin_port(int val) { sin_port = (htons(val)); }
        /** Access s_addr
         * \return The current value of s_addr
         */
        unsigned long Gets_addr() { return s_addr; }

 //       char GetMessage() { return message; }
        /** Set s_addr
         * \param val New value to set
         */
        void Sets_addr(unsigned long val) { s_addr = val; }
        /** Access message_size
         * \return The current value of message_size
         */
        int Getmessage_size() { return message_size; }
        /** Set message_size
         * \param val New value to set
         */
        void Setmessage_size(int val) { message_size = val; }
    protected:
    private:
        unsigned short port; //!< Member variable "port"
        short sin_family; //!< Member variable "sin_family"
        u_short sin_port; //!< Member variable "sin_port"
        unsigned long s_addr; //!< Member variable "s_addr"
        int message_size; //!< Member variable "message_size"
        int sockfd;
        int newsockfd;
        int n;
        int clilen; // stores the size of the client
        socklen_t fromlen; // number of characters read/written
        char buf[MESSAGE_SIZE]; // server reads input to this buffer
        char message[MESSAGE_SIZE];
        struct sockaddr_in serv_addr; // server addresses
        struct sockaddr_in cli_addr; // client addresses
};

#endif // SOCKET_H
