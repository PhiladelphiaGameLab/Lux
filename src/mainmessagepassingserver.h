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
 */#ifndef MAINMESSAGEPASSINGSERVER_H
#define MAINMESSAGEPASSINGSERVER_H

#include <cstdlib>
#include <iostream>
#include "mongo/client/dbclient.h"


class MainMessagePassingServer
{
    public:
        /** Default constructor */
        MainMessagePassingServer(int port, std::string configFile);
        /** Default destructor */
        virtual ~MainMessagePassingServer();
        /** Copy constructor
         *  \param other Object to copy from
         */
        MainMessagePassingServer(const MainMessagePassingServer& other);
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        MainMessagePassingServer& operator=(const MainMessagePassingServer& other);

        int createThreads(int numThreads);

        /** Access socket
         * \return The current value of socket
         */
        Socket Getsocket() { return socket; }
        /** Set socket
         * \param val New value to set
         */
        void Setsocket(Socket val) { socket = val; }
        /** Access AP
         * \return The current value of AP
         */
        int GetAP() { return AP; }
        /** Set AP
         * \param val New value to set
         */
        void SetAP(int val) { AP = val; }
        /** Access HMBL
         * \return The current value of HMBL
         */
        HashMapBasedLocation GetHMBL() { return HMBL; }
        /** Set HMBL
         * \param val New value to set
         */
        void SetHMBL(HashMapBasedLocation val) { HMBL = val; }
        /** Access cli_addr
         * \return The current value of cli_addr
         */
        struct sockaddr_in Getcli_addr() { return cli_addr; }
        /** Set cli_addr
         * \param val New value to set
         */
        void Setcli_addr(struct sockaddr_in val) { cli_addr = val; }
    protected:
    private:
        Socket socket; //!< Member variable "socket"
        int AP; //!< Member variable "AP"
        HashMapBasedLocation HMBL; //!< Member variable "HMBL"
        struct sockaddr_in cli_addr; //!< Member variable "cli_addr"
};

#endif // MAINMESSAGEPASSINGSERVER_H
