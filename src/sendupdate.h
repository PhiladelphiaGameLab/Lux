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
 */#ifndef SENDUPDATE_H
#define SENDUPDATE_H


class SendUpdate
{
    public:
        /** Default constructor */
        SendUpdate(int sendFIFO, HashMapBasedLocation* HMBL);
        /** Default destructor */
        virtual ~SendUpdate();
        /** Copy constructor
         *  \param other Object to copy from
         */
        SendUpdate(const SendUpdate& other);
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        SendUpdate& operator=(const SendUpdate& other);

        static void createSUT(int sendFIFO, HashMapBasedLocation* HMBL)

        /** Access sendSocket
         * \return The current value of sendSocket
         */
        Socket GetsendSocket() { return sendSocket; }
        /** Set sendSocket
         * \param val New value to set
         */
        void SetsendSocket(Socket val) { sendSocket = val; }
        /** Access sendFIFO
         * \return The current value of sendFIFO
         */
        int GetsendFIFO() { return sendFIFO; }
        /** Set sendFIFO
         * \param val New value to set
         */
        void SetsendFIFO(int val) { sendFIFO = val; }
        /** Access sendingMessage
         * \return The current value of sendingMessage
         */
        JSObject GetsendingMessage() { return sendingMessage; }
        /** Set sendingMessage
         * \param val New value to set
         */
        void SetsendingMessage(JSObject val) { sendingMessage = val; }
        /** Access message
         * \return The current value of message
         */
        JSObject Getmessage() { return message; }
        /** Set message
         * \param val New value to set
         */
        void Setmessage(JSObject val) { message = val; }
        /** Access toBeSent
         * \return The current value of toBeSent
         */
        std::string GettoBeSent() { return toBeSent; }
        /** Set toBeSent
         * \param val New value to set
         */
        void SettoBeSent(std::string val) { toBeSent = val; }
    protected:
    private:
        Socket sendSocket; //!< Member variable "sendSocket"
        int sendFIFO; //!< Member variable "sendFIFO"
        JSObject sendingMessage; //!< Member variable "sendingMessage"
        JSObject message; //!< Member variable "message"
        std::string toBeSent; //!< Member variable "toBeSent"
};

#endif // SENDUPDATE_H
