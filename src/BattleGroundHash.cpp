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

#include "battlegroundhash.h"
 	//CONSTRUCTORS
		LocationBasedHash::LocationBasedHash(int mapw, int maph, int col, int row){
			mapwidth = mapw;
			mapheight = maph;
			columns = col;
			rows = row;
			
			hashTable = new std::list<struct sockaddr_in>[columns * rows];
		}

		LocationBasedHash::LocationBasedHash(){
			mapwidth = 100;
			mapheight = 100;
			columns = 20;
			rows = 20;
			
			hashTable = new std::list<struct sockaddr_in>[columns * rows];
		}

//DESTRUCTOR
		LocationBasedHash::~LocationBasedHash(){
			for(int i = 0; i < columns * rows; i++){
				hashTable[i].clear();
			}

			free(hashTable);
		}

//HELPER
		int LocationBasedHash::hashFunction(int val1, int val2)){
			return val1%columns + (val2%rows)*columns;
		}

//GETTERS
		std::list<struct sockaddr_in>* LocationBasedHash::get(int x, int y){
			std::list<struct sockaddr_in>* retVal = new std::list<struct sockaddr_in>();
			int i = hashFunction(x, y);

			retVal.merge(hashTable[i]);
			retVal.merge(hashTable[i-1]);
			retVal.merge(hashTable[i+1]);
			retVal.merge(hashTable[i+columns]);
			retVal.merge(hashTable[i+columns-1]);
			retVal.merge(hashTable[i+columns+1]);
			retVal.merge(hashTable[i-columns]);
			retVal.merge(hashTable[i-columns-1]);
			retVal.merge(hashTable[i-columns+1]);
			
			return retVal;
		}

		std::list<struct sockaddr_in> LocationBasedHash::getIndex(int idx){
			return hashTable(idx);
		}

		std::list<struct sockaddr_in>[] getSockets(){
			return hashTable;
		}

//SETTERS
		void LocationBasedHash::updateLocation(int x, int y, struct sockaddr_in location){
			hashTable[hashFunction(x,  y)].push_back(location);
		}

//OPERATORS
		// LinkedList& LocationBasedHash::operator[](int idx){

		// }

		// const LinkedList& LocationBasedHash::operator[](int idx) const{

		// }
}

#endif // SERVER_H_INCLUDED