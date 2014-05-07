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

#include "locbasedhashmap.h"
 	//CONSTRUCTORS
		HMBL::HMBL(int mapw, int maph, int col, int row){
			mapwidth = mapw;
			mapheight = maph;
			columns = col;
			rows = row;
			
			hashTable = new std::list<struct sockaddr_in> [columns * rows];
		}

		HMBL::HMBL(){
			HMBL(100, 100, 20, 20);
		}

//DESTRUCTOR
		HMBL::~HMBL(){
			for(int i = 0; i < columns * rows; i++){
				hashTable[i].clear();
			}

			delete[] hashTable;
		}

//HELPER
		int HMBL::hashFunction(int val1, int val2){
			return val1%columns + (val2%rows)*columns;
		}

//GETTERS
		std::list<struct sockaddr_in>* HMBL::getSocketLists(int x, int y){
			std::list<struct sockaddr_in>* retVal = new std::list<struct sockaddr_in>;
			int i = hashFunction(x, y);

			retVal->merge(hashTable[i]);
			if(i % columns != 0)
				retVal->merge(hashTable[i-1]);
			if(i % columns != columns-1)
				retVal->merge(hashTable[i+1]);

			if(i < columns*(rows - 1)){
				retVal->merge(hashTable[i+columns]);
				if(i % columns != 0)
					retVal->merge(hashTable[i+columns-1]);
				if(i % columns != columns-1)
					retVal->merge(hashTable[i+columns+1]);
			}
			if(i > columns){
				retVal->merge(hashTable[i-columns]);
				if(i % columns != 0)
					retVal->merge(hashTable[i-columns-1]);
				if(i % columns != columns-1)
					retVal->merge(hashTable[i-columns+1]);
			}
			return retVal;
		}

		std::list<struct sockaddr_in> HMBL::getIndex(int idx){
			return hashTable[idx];
		}

		std::list<struct sockaddr_in>* HMBL::getSockets(){
			return hashTable;
		}

//SETTERS
		void HMBL::updateLocation(int x, int y, struct sockaddr_in location){
			hashTable[hashFunction(x,  y)].push_back(location);
		}

		void HMBL::removeUserFromLocation(){
			//NOT SURE HOW TO EXACTLY TO DO THIS
		}

//OPERATORS - Would like to add [] based operators to allow people to get from the HashTable easier
		std::list<struct sockaddr_in>& HMBL::operator[](int idx){
			hashTable[idx];
		}

		const std::list<struct sockaddr_in>& HMBL::operator[](int idx) const{
			hashTable[idx];
		}


		//---------------------------------------

		HMBS::HMBS(int gt, int mog){
			gameType = gt;
			maxOpenGames = mog;

			battlegrounds = new std::list<BattleGround> [mog];
		}

		HMBS::HMBS(){
			HMBS(1, 10); //DEFAULTS
		}

		HMBS::~HMBS(){
			for(int i = 0; i < maxOpenGames; i++){
				battlegrounds[i].clear();
			}

			delete[] battlegrounds;
		}

		int HMBS::hashFunction(int val){
			//BLANK FOR NOW UNTIL FIGURE THIS OUT
			//TODO: FIGURE OUT HASH FUNCTION

			return 0;
		}

		BattleGround HMBS::getBGT(int x, int y){
			return battlegrounds->front();  //for now, not sure how to do this
		}

		BattleGround HMBS::getIndex(int idx){
			battlegrounds[idx];
		}

		void HMBS::addBGT(int x, int y){
			battlegrounds->add(new BattleGround());
		}

		BattleGround& HMBS::operator[](int idx){
			battlegrounds[idx];
		}

  		const BattleGround& HMBS::operator[](int idx) const{
			battlegrounds[idx];
  		}
