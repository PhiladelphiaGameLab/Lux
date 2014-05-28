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

/**********  The Double Linked List Struct (for reference) *********/
//struct CNode
//{struct Node
//{
//	T sock;
//	int lastBuck = 0;
//	struct Node *Next;
//	struct Node *Prev;
//};}
//*Prev;
//*Next;
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
	hashTable[bucketTotal / 2]; //HashMap of Clients

	clearHMBL();
}

//Default Constructor
template <class T>
HMBL<T>::HMBL(){
	HMBL<T>(100, 100, 20, 20);
}

//Adds Client location to hashTable
template <class T>
void HMBL<T>::update(T nsock, int euid, int x, int y){

	int bucketNum = findBucket(x, y);
	int hashKey = euid % (bucketTotal / 2);

	struct Node *newNode = checkForCollision(euid, hashKey);

	//Check to remove previous instance; this whole thing is questionable
	if (newNode->lastBuck != 0){ //Checks to see if there is an old instance in existence, if not this shouldn't occur
		//Time for some brute force if-else
		if (newNode->Prev == 0 && newNode->Next != 0){
			arrMap[newNode->lastBuck] = newNode->Next;
		}
		if (newNode->Prev != 0 && newNode->Next == 0){
			newNode->Prev->Next = 0;
		}
		if (newNode->Prev != 0 && newNode->Next != 0){
			newNode->Prev->Next = newNode->Next;
			newNode->Next->Prev = newNode->Prev;
		}
		else{ //Only occurs is prev and next of Node walk = 0
			arrMap[newNode->lastBuck] = 0;
		}
	}

	//Storing inside of the arrMap/hashTable
	if (arrMap[bucketNum] != 0){ //Occurs when another client is in this bucket
		struct Node *tempMap;
		

		tempMap = arrMap[bucketNum]; // tempMap stores original client in the bucket. Don't know if arrMap should be pointer
		arrMap[bucketNum] = newNode;
		newNode->Next = tempMap;
		tempMap->Prev = newNode;
		newNode->sock = nsock;
		newNode->euid = euid;
		newNode->lastBuck = bucketNum;
	}
	else{
		arrMap[bucketNum] = newNode;
		newNode->sock = nsock;
		newNode->euid = euid;
		newNode->lastBuck = bucketNum;
	}
}


//Finds which Bucket they are in (assume that the 0 bucket begins at (0,0))
template <class T>
int HMBL<T>::findBucket(int x, int y){
	float bucket_dimensionX = mapwidth / columns;
	float bucket_dimensionY = mapheight / rows;
	int xBuckets = x / bucket_dimensionX;
	int yBuckets = y / bucket_dimensionY;

	int bucketNum = (xBuckets * rows) - (rows - yBuckets - 1);
	return bucketNum;
}

template <class T>
Node HMBL<T>::checkForCollision(int euid, int hashkey){//Checks for a collision in the hashMap and adds it to the CollisionNode if there is one
	struct Node *newNode;
	struct CNode *cwalk = hashTable[hashKey];
	bool addCollInst = true;

	if (hashTable[hashKey]->Base->euid == 0 || hashTable[hashKey]->Base->euid == euid){//this is the instance or there is no instance 
		newNode = hashTable[hashKey]->Base;
	}
	else if (hashTable[hashKey]->Base->euid != euid){//Collision detected
		while (cwalk->Next != 0){
			cwalk = cwalk->Next;
			if (cwalk->Base->euid == euid){
				addCollInst = false;
				newNode = cwalk->Base;
			}
		}
		if (addCollInst){//Not sure about this; definitely recheck
			struct* CNode cnewNode
		    cwalk->Next = cnewNode;
			cnewNode->Prev = cwalk;
		}
	}
	
	return newNode;
}

template <class T>
void HMBL<T>::clearHMBL(){
	for (int i = 0; i < bucketTotal; i++){
		arrMap[i] = 0;
		if (i < (bucketTotal / 2)){
			hashTable[i]->Next = 0;
			hashTable[i]->Prev = 0;
			hashTable[i]->Base->sock = 0;
			hashTable[i]->Base->euid = 0;
			hashTable[i]->Base->Next = 0;
			hashTable[i]->Base->Prev = 0;
			//Might need this, but probably not: hashTable[i]->Base = 0;
		}
	}
}