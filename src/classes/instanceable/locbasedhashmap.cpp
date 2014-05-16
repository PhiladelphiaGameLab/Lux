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
			
			hashTable = new std::list<LocData<T> > [columns * rows];
		}

		template <class T>
		HMBL<T>::HMBL(){ //DEFAULT CONSTRUCTOR
			HMBL<T>(100, 100, 20, 20);
		}
		

		template <class T>
		LocData<T>::LocData(int expire, T data){
			this.expire = expire;
			this.data = data;
		}

		template <class T>
		LocData<T>::LocData(T data){
			LocData<T>(5, data);
		}
		

//DESTRUCTOR
		template <class T>
		HMBL<T>::~HMBL(){
			for(int i = 0; i < columns * rows; i++){
				hashTable[i].clear();
			}

			delete[] hashTable;
		}

		template <class T>
		LocData<T>::~LocData(){
			delete data;
		}

//GETTERS
		template <class T>
		std::list<LocData<T> >* HMBL<T>::getSocketLists(int loc){
			if(idx >= columns*rows || idx < 0)
				return NULL;

			std::list<LocData<T> >* retVal = new std::list<LocData<T> >;

			copyIntoList(retVal, hashTable[loc]);
			if(loc % columns != 0)
				copyIntoList(retVal, (hashTable[loc-1]);
			if(loc % columns != columns-1)
				copyIntoList(retVal, (hashTable[loc+1]);

			if(loc < columns*(rows - 1)){
				copyIntoList(retVal, (hashTable[loc+columns]);
				if(loc % columns != 0)
					copyIntoList(retVal, (hashTable[loc+columns-1]);
				if(loc % columns != columns-1)
					copyIntoList(retVal, (hashTable[loc+columns+1]);
			}
			if(loc > columns){
				copyIntoList(retVal, (hashTable[loc-columns]);
				if(loc % columns != 0)
					copyIntoList(retVal, (hashTable[loc-columns-1]);
				if(loc % columns != columns-1)
					copyIntoList(retVal, (hashTable[loc-columns+1]);
			}

			return retVal;
		}

		template <class T>
		std::list<LocData<T> > HMBL<T>::getIndex(int idx){
			if(idx >= columns*rows || idx < 0)
				return NULL;
			return hashTable[idx];
		}

		template <class T>
		std::list<LocData<T> >* HMBL<T>::getSockets(){
			return hashTable;
		}

//SETTERS
		template <class T>
		void HMBL<T>::add(int loc, T value){
			if(hashTable[loc] == NULL)
				hashTable[loc] = new std::list<LocData<T> >;

			LocData<T> tempVal = new LocData<T>(5, value);
			
			hashTable[loc].push_back(tempVal);
		}

		template <class T>
		void HMBL<T>::removeExpiredObjects(){
			for(int i = 0; i < columns*rows; i++){
				typename std::list<LocData<T> >::iterator itB = hashTable[i]->begin();
				typename std::list<LocData<T> >::iterator itE = hashTable[i]->end();
				while(itB != itE){
					if(*itB.expire == 0){
						hashTable[i]->erase(itB);
					}else{//check for duplicates if none, move to back of list and erase this one
						*itB.expire--;
						typename std::list<LocData<T> >::iterator itR = itE;
						while(itR != itB){
							if(*itR.data == *itB.data)
								break;
							itR--;
						}

						if(itR != itB){
							hashTable[i]->erase(itB);
						}else{
							hashTable[i]->push_back(*itB);
							hashTable[i]->erase(itB);
						}
					}
					itB++;
				}
			}
		}

//HELPERS
		void HMBL<T>::copyIntoList(std::list<LocData<T> >* into, const std::list<LocData<T> > from){
			typename std::list<LocData<T> >::iterator itB = hashTable[i]->begin();
			typename std::list<LocData<T> >::iterator itE = hashTable[i]->end();

			for(; itB != itE; itB++){
				into->emplace(into->begin(), itB.expire, itB.data);
			}
		}

//OPERATORS - Would like to add [] based operators to allow people to get from the HashTable easier
		template <class T>
		std::list<LocData<T> >& HMBL<T>::operator[](int idx){
			if(idx >= columns*rows || idx < 0)
				return NULL;
			return hashTable[idx];
		}

		template <class T>
		const std::list<LocData<T> >& HMBL<T>::operator[](int idx) const{
			if(idx >= columns*rows || idx < 0)
				return NULL;
			return hashTable[idx];
		}
