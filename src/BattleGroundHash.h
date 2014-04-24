#ifndef LBHM_H_INCLUDED
#define LBHM_H_INCLUDED

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
 * - Mike
 */

#include <stdlib.h>
#include <list>
#include "socket.h"

class LocationBasedHash{
	private:
		int mapwidth;
		int mapheight;
		int columns;
		int rows;
		std::list<struct sockaddr_in>[] hashTable;

	public:
		LocationBasedHash(int mapw, int maph, int col, int row);
		LocationBasedHash();
		~LocationBasedHash();

		int hashFunction(int val1, int val2);
		std::list<struct sockaddr_in>* get(int x, int y);
		std::list<struct sockaddr_in> getIndex(int idx);
		std::list<struct sockaddr_in>[] getSockets();
		void updateLocation(int x, int y, struct sockaddr_in location);
		// std::list<struct sockaddr_in>& operator[](int idx);
  	// const std::list<struct sockaddr_in>& operator[](int idx) const;
}

class BattleGroundHash{
	private:
		int gameType;
		int maxOpenGames;
		LocationBasedHash[] hashTable;

	public:
		BattleGroundHash(int gt, int mog);
		BattleGroundHash();
		~BattleGroundHash();

		int hashFunction(int val);
		LocationBasedHash get(int x, int y);
		LocationBasedHash getIndex(int idx);
		void addBGT(int x, int y);
		// LocationBasedHash& operator[](int idx);
  	// const LocationBasedHash& operator[](int idx) const;
}

#endif // SERVER_H_INCLUDED