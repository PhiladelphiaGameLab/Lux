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
 */#ifndef BATTLEGROUND_H
#define BATTLEGROUND_H


class BattleGround
{
    public:
        /** Default constructor */
        BattleGround(int[6] location, char[20] name, int id, int port, HashMapBasedLocation* HMBL, struct DBC DBConnection, int AP);
        /** Default destructor */
        virtual ~BattleGround();
        /** Copy constructor
         *  \param other Object to copy from
         */
        BattleGround(const BattleGround& other);
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        BattleGround& operator=(const BattleGround& other);

        static void createBGT();
        void newConnectionsThread();

        /** Access location
         * \return The current value of location
         */
        int[6] Getlocation() { return location; }
        /** Access name
         * \return The current value of name
         */
        char[20] Getname() { return name; }
        /** Access id
         * \return The current value of id
         */
        int Getid() { return id; }
        /** Access port
         * \return The current value of port
         */
        int Getport() { return port; }
        /** Access HMBL
         * \return The current value of HMBL
         */
        HashMapBasedLocation* GetHMBL() { return HMBL; }
        /** Set HMBL
         * \param val New value to set
         */
        void SetHMBL(HashMapBasedLocation* val) { HMBL = val; }
        /** Access DBConnection
         * \return The current value of DBConnection
         */
        struct DBC GetDBConnection() { return DBConnection; }
        /** Set DBConnection
         * \param val New value to set
         */
        void SetDBConnection(struct DBC val) { DBConnection = val; }
        /** Access AP
         * \return The current value of AP
         */
        int GetAP() { return AP; }
        /** Set AP
         * \param val New value to set
         */
        void SetAP(int val) { AP = val; }
        /** Access SP
         * \return The current value of SP
         */
        int GetSP() { return SP; }
        /** Access BCP
         * \return The current value of BCP
         */
        int GetBCP() { return BCP; }
        /** Access NCP
         * \return The current value of NCP
         */
        int GetNCP() { return NCP; }
        /** Access recieveSocket
         * \return The current value of recieveSocket
         */
        Socket GetrecieveSocket() { return recieveSocket; }
    protected:
    private:
        int[6] location; //!< Member variable "location"
        char[20] name; //!< Member variable "name"
        int id; //!< Member variable "id"
        int port; //!< Member variable "port"
        HashMapBasedLocation* HMBL; //!< Member variable "HMBL"
        struct DBC DBConnection; //!< Member variable "DBConnection"
        int AP; //!< Member variable "AP"
        int SP; //!< Member variable "SP"
        int BCP; //!< Member variable "BCP"
        int NCP; //!< Member variable "NCP"
        Socket recieveSocket; //!< Member variable "recieveSocket"
};

#endif // BATTLEGROUND_H
