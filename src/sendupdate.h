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
    protected:
    private:
};

#endif // SENDUPDATE_H
