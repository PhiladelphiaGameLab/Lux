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
#include "battleground.h"

class HMBL{
	private:
		int mapwidth;
		int mapheight;
		int columns;
		int rows;
		std::list<struct sockaddr_in>* hashTable;

		void removeUserFromLocation();

	public:
		HMBL(int mapw, int maph, int col, int row);
		HMBL();
		~HMBL();

		int hashFunction(int val1, int val2);
		std::list<struct sockaddr_in>* getSocketLists(int x, int y);
		std::list<struct sockaddr_in> getIndex(int idx);
		std::list<struct sockaddr_in>* getSockets();
		void updateLocation(int x, int y, struct sockaddr_in location);
		std::list<struct sockaddr_in>& operator[](int idx);
  		const std::list<struct sockaddr_in>& operator[](int idx) const;
};

class HMBS{
	private:
		int gameType;
		int maxOpenGames;
		std::list<BattleGround>* battlegrounds;

	public:
		HMBS(int gt, int mog);
		HMBS();
		~HMBS();

		int hashFunction(int val);
		BattleGround getBGT(int x, int y);
		BattleGround getIndex(int idx);
		void addBGT(int x, int y);
		BattleGround& operator[](int idx);
  		const BattleGround& operator[](int idx) const;
};

#endif // LBHM_H_INCLUDED
