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
 * - Mike Oak
 */

#include "locbasedhashmap.h"
 	//CONSTRUCTORS
		template <class T>
		HMBL<T>::HMBL(int mapw, int maph, int col, int row){
			mapwidth = mapw;
			mapheight = maph;
			columns = col;
			rows = row;
			
			hashTable = new std::list<T> [columns * rows];
		}

		template <class T>
		HMBL<T>::HMBL(){ //DEFAULT CONSTRUCTOR
			HMBL<T>(100, 100, 20, 20);
		}

//DESTRUCTOR
		template <class T>
		HMBL<T>::~HMBL(){
			for(int i = 0; i < columns * rows; i++){
				hashTable[i].clear();
			}

			delete[] hashTable;
		}

//GETTERS
		template <class T>
		std::list<T>* HMBL<T>::getSocketLists(int loc){
			std::list<T>* retVal = new std::list<T>;

			retVal->merge(hashTable[loc]);
			if(loc % columns != 0)
				retVal->merge(hashTable[loc-1]);
			if(loc % columns != columns-1)
				retVal->merge(hashTable[loc+1]);

			if(loc < columns*(rows - 1)){
				retVal->merge(hashTable[loc+columns]);
				if(loc % columns != 0)
					retVal->merge(hashTable[loc+columns-1]);
				if(loc % columns != columns-1)
					retVal->merge(hashTable[loc+columns+1]);
			}
			if(loc > columns){
				retVal->merge(hashTable[loc-columns]);
				if(loc % columns != 0)
					retVal->merge(hashTable[loc-columns-1]);
				if(loc % columns != columns-1)
					retVal->merge(hashTable[loc-columns+1]);
			}
			return retVal;
		}

		template <class T>
		std::list<T> HMBL<T>::getIndex(int idx){
			return hashTable[idx];
		}

		template <class T>
		std::list<T>* HMBL<T>::getSockets(){
			return hashTable;
		}

//SETTERS
		template <class T>
		void HMBL<T>::add(int loc, T value){
			if(hashTable[loc] == NULL)
				hashTable[loc] = new std::list<T>; 
			
			hashTable[loc].push_back(value);
		}

		template <class T>
		void HMBL<T>::removeUserFromLocation(){
			//NOT SURE HOW TO EXACTLY TO DO THIS
		}

//OPERATORS - Would like to add [] based operators to allow people to get from the HashTable easier
		template <class T>
		std::list<T>& HMBL<T>::operator[](int idx){
			hashTable[idx];
		}

		template <class T>
		const std::list<T>& HMBL<T>::operator[](int idx) const{
			hashTable[idx];
		}
