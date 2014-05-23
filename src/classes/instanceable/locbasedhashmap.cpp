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

#include "HMBL.h"

/**********  The Double Linked List Struct *********/
template <class T>
struct Node
{
	T sock;
	int lastBuck = 0;
	struct Node *next;
	struct Node *prev;
};
/**************** End of Struct *******************/
/**************************************************/

//Constructor
template <class T>
HMBL<T>::HMBL(int mapw, int maph, int col, int row){
		mapwidth = mapw;
		mapheight = maph;
		columns = col;
		rows = row;
		bucketTotal = row * col;
	 
		arrMap[bucketTotal]; //Array of Pointers representing each bucket
		hashTable[bucketTotal/2]; //HashMap of Clients
		
		for(int i = 0; i < bucketTotal; i++){
			*arrMap[i] = 0;
			if(i < (bucketTotal/2))
				hashTable[i]->sock = 0;
				hashTable[i]->next = 0;
				hashTable[i]->prev = 0;
		}
		//struct Node *head; not sure if necessary
	}

//Default Constructor
template <class T>
HMBL<T>::HMBL(){ 
		HMBL<T>(100, 100, 20, 20);
	}

//Adds Client location to hashTable
template <class T>
HMBL<T>::update(T nsock, int euid, int x, int y){
	
	int bucketNum = findBucket(x, y);
	int hashKey = euid%(bucketTotal/2);

	//Check to remove previous instance; this whole thing is questionable
	if(hashTable[hashKey]->lastBuck != 0){ //Checks to see if there is an old instance in existence, if not this shouldn't occur
		struct Node *walk;
		walk = hashTable[hashKey];
		while(walk->sock != nsock){ //Walks through attached nodes if there is more than one in a bucket 
			walk = walk->next;
		}
		struct Node *temp;
		//Time for some brute force if-else
		if(walk->prev == 0 && walk->next != 0){
			*arrMap[walk->lastBuck] = walk->next;
		}
		if(walk->prev != 0 && walk->next == 0){
			walk->prev->next = 0;
		}
		if(walk->prev != 0 && walk->next !=0){
			//Hard Case, need to fix.
		}
		else{ //Only occurs is prev and next of Node walk = 0
			*arrMap[walk->lastBuck] = 0;
		}

	}



	//Storing inside of the arrMap/hashTable
	if(*arrMap[bucketNum] != 0){ //Occurs when another client is in this bucket
		struct Node *tempMap;
		tempMap = *arrMap[bucketNum]; // tempMap stores original client in the bucket. Don't know if arrMap should be pointer
		*arrMap[bucketNum] = hashTable[hashKey]; 
		hashTable[hashKey]->next = tempMap;
		tempMap->prev = hashTable[hashKey];
		hashTable[hashKey]->sock = nsock;
		hashTable[hashKey]->lastBuck = bucketNum;
	}
	else{
		*arrMap[bucketNum] = hashTable[hashKey];
		hashTable[hashKey]->sock = nsock;
		hashTable[hashKey]->lastBuck = bucketNum;
	}
}

//Finds which Bucket they are in (assume that the 0 bucket begins at (0,0))
template <class T>
int HMBL<T>::findBucket(int x, int y){
	float bucket_dimensionX = mapwidth/col;
	float bucket_dimensionY = mapheight/row;
	int xBuckets = x/bucket_dimensionX;
	int yBuckets = y/bucket_dimensionY;

	int bucketNum = (xBuckets * row) - (row - yBuckets - 1);
	return bucketNum; 
}


template <class T>
template <class T>
template <class T>
template <class T>