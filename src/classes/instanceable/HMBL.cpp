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
* - Jeff Ramspacher
*/

#include "HMBL.h"
#include <stdio.h>
#include <iostream>
#include <vector>

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
HMBL<T>::HMBL(int mapw, int maph, int col, int row, std::string pipeLocation){
	mapwidth = mapw;
	mapheight = maph;
	columns = col;
	rows = row;
	bucketTotal = row * col;

	arrMap = new Node<T>*[bucketTotal]; //Array of Pointers representing each bucket
	hashTable = new CNode<T>*[bucketTotal / 2]; //HashMap of Clients

	clearHMBL();
	
	pipeFD = open(pipeLocation.c_str(), O_WRONLY);
}

//Default Constructor
template <class T>
HMBL<T>::HMBL(){
	HMBL(100, 100, 1, 1, "temp/pipes/shitfuckmotherfuckerthisbetterwork");
}

//Adds Client location to hashTable
template <class T>
void HMBL<T>::update(T nsock, int euid, int x, int y, int rad){

	int bucketNum = findBucket(x, y);
	int hashKey = euid % (bucketTotal / 2);

	Node<T> *newNode = checkForCollision(euid, hashKey);

	int lastBuck = newNode->currBuck;

	//Checks to see if there is an old instance in existence, if not this shouldn't occur
	if (newNode->currBuck != 0){ 
		if (newNode->Prev == 0 && newNode->Next != 0){
			arrMap[newNode->currBuck] = newNode->Next;
			newNode->Next->Prev = 0;
			newNode->Next = 0;
		}else if (newNode->Prev != 0 && newNode->Next == 0){
			newNode->Prev->Next = 0;
			newNode->Prev = 0;
		}else if (newNode->Prev != 0 && newNode->Next != 0){
			newNode->Prev->Next = newNode->Next;
			newNode->Next->Prev = newNode->Prev;
			newNode->Next = 0;
			newNode->Prev = 0;
		}
		else{ //Only occurs is prev and next of Node walk = 0
			arrMap[newNode->currBuck] = 0;
		}
	}

	//Storing inside of the arrMap/hashTable
	if (arrMap[bucketNum] != 0){ //Occurs when another client is in this bucket
		Node<T> *tempMap;

		tempMap = arrMap[bucketNum]; // tempMap stores original client in the bucket.
		arrMap[bucketNum] = newNode;
		newNode->Next = tempMap;
		tempMap->Prev = newNode;
		newNode->sock = nsock;
		newNode->euid = euid;
		newNode->currBuck = bucketNum;
	}else{
		arrMap[bucketNum] = newNode;
		newNode->sock = nsock;
		newNode->euid = euid;
		newNode->currBuck = bucketNum;
	}

	if (lastBuck == bucketNum){ //for efficiency, so that it doesn't go through pipe process if client didn't move

	}else
		pipeInfo(x, y, rad, lastBuck);
}

//Internal Function for finding the Bucket number: calls static method
template <class T>
int HMBL<T>::findBucket(int x, int y){
	return getBucket(x, y, mapwidth, mapheight, columns, rows);
}

//Checks for a collision in the hashMap and adds it to the CollisionNode if there is one
template <class T>
Node<T>* HMBL<T>::checkForCollision(int euid, int hashKey){
	Node<T> *newNode = new Node<T>;
	CNode<T> *cwalk = hashTable[hashKey];
	bool addCollInst = true;

	if (hashTable[hashKey]->Base->euid == 0 || hashTable[hashKey]->Base->euid == euid){ //this checks if this is the instance or if there is no instance 
		newNode = hashTable[hashKey]->Base;
	}else if (hashTable[hashKey]->Base->euid != euid){ //Collision detected
		while (cwalk->Next != 0){
			cwalk = cwalk->Next;
			if (cwalk->Base->euid == euid){
				addCollInst = false;
				newNode = cwalk->Base;
			}
		}
		if (addCollInst){  
			struct CNode<T>* cnewNode = new CNode<T>;
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
			hashTable[i] = new CNode<T>;
			hashTable[i]->Base = new Node<T>;
			hashTable[i]->Next = 0;
			hashTable[i]->Prev = 0;
			hashTable[i]->Base->sock = 0;
			hashTable[i]->Base->euid = 0;
			hashTable[i]->Base->Next = 0;
			hashTable[i]->Base->Prev = 0;
			hashTable[i]->Base->lastBuck = -1;
		}
	}
}

template <class T>
void HMBL<T>::revealHMBL(){
	for (int i = 0; i < bucketTotal; i++){
		if (arrMap[i] != 0)
			std::cout << " arrMap[" << i << "] contains " << arrMap[i] << " points to user " << arrMap[i]->euid << std::endl;
		if (i < (bucketTotal / 2)){
			if (hashTable[i]->Base->euid != 0){
				std::cout << " hashTable[" << i << "] contains user " << hashTable[i]->Base->euid << std::endl;
				std::cout << " " << hashTable[i]->Base->lastBuck << " " << hashTable[i]->Base->sock << " Prev:" << hashTable[i]->Base->Prev << " Next:" << hashTable[i]->Base->Next << std::endl;
			}
		}
	}
}

template <class T>
std::vector<Node<T>*> HMBL<T>::get_clients(int xloc, int yloc, int rad){
	std::vector<Node<T>*> clients; //will be returned by this function and contain pointers to affected clients
	std::vector<int> proxBucks; //contains the list of buckets in the items proximity

	proxBucks = surroundings(xloc, yloc, rad, mapwidth, mapheight, columns, rows);

	int thresh = proxBucks.size();
	Node<T>* walk;

	for (int i = 0; i < thresh; i++){
		walk = arrMap[proxBucks[i]];
		while (walk != 0){
			clients.push_back(walk);
			walk = walk->Next;
		}
	}

	return clients;
}

template <class T>
std::vector<int> HMBL<T>::surroundingsFromCurrBucket(int currentBucket, int rad, int mapx, int mapy, int columns, int rows){
	std::vector<int> surroundings;

	if (currentBucket == -1){ //Used to ensure that this isn't the first update for the client

	}else{
		int x = currentBucket / rows;
		int y = currentBucket % rows;
		int xval;
		int yval;

		//Algorithm for finding the surrounding relative buckets while taking the edges of the map into account
		for (int i = rad; i >= 0; i--){
			for (int j = rad; j >= 0; j--){

				xval = x - i;
				yval = y - j;

				if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
					surroundings.push_back(backToBuck(xval, yval, rows));
				}
			}
			for (int j = 1; j <= rad; j++){

				xval = x - i;
				yval = y + j;

				if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
					surroundings.push_back(backToBuck(xval, yval, rows));
				}
			}
		}
		for (int i = 1; i <= rad; i++){
			for (int j = rad; j >= 0; j--){

				xval = x + i;
				yval = y - j;

				if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
					surroundings.push_back(backToBuck(xval, yval, rows));
				}
			}
			for (int j = 1; j <= rad; j++){

				xval = x + i;
				yval = y + j;

				if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
					surroundings.push_back(backToBuck(xval, yval, rows));
				}
			}
		}

		int thresh = surroundings.size(); //The following statements are all for testing
		std::cout << " Size: " << surroundings.size() << std::endl;
		for (int k = 0; k < thresh; k++){
			std::cout << " [" << k << "] = " << surroundings[k] << std::endl;
		}
	}
	return surroundings;
}

//Method that pipes to the SNR thread
template <class T>
void HMBL<T>::pipeInfo(int x, int y, int rad, int lastBuck){
	std::vector<int> impSurr;
	std::vector<int> newSurr = surroundings(x, y, rad, mapwidth, mapheight, columns, rows);

	if (lastBuck == -1){
		impSurr = newSurr;
	}else{
		std::vector<int> oldSurr = surroundingsFromCurrBucket(lastBuck, rad, mapwidth, mapheight, columns, rows);

		bool addOutstanding;

		for (int i = 0; i < newSurr.size(); i++){
			addOutstanding = true;
			for (int j = 0; j < oldSurr.size(); j++){
				if (newSurr[i] == oldSurr[j])
					addOutstanding = false;
			}
			if (addOutstanding)
				impSurr.push_back(newSurr[i]);
		}
	}

	s_SNRMessage *newSurrBuck = new s_SNRMessage;
	newSurrBuck->newBuckList = impSurr;
	//newSurrBuck->socket = ;
	write(pipeFD, newSurrBuck, sizeof(newSurrBuck));

}

//-------------------------STATIC FUNCTIONS--------------------------------
template <class T>
int HMBL<T>::getBucket(int xloc, int yloc, int mapx, int mapy, int columns, int rows){
	int bucket_dimensionX = mapx / columns;
	int bucket_dimensionY = mapy / rows;
	int xBuckets = xloc / bucket_dimensionX;
	int yBuckets = yloc / bucket_dimensionY;
	int bucketNum;
	if (yBuckets > 0)
		bucketNum = ((xBuckets * rows) - (rows % yBuckets));// - 1 is for indexing the array of buckets
	else
		bucketNum = (xBuckets * rows - 0);
	return bucketNum;
}

template <class T>
std::vector<int> HMBL<T>::surroundings(int xloc, int yloc, int rad, int mapx, int mapy, int columns, int rows){
	std::vector<int> surroundings;

	int currentBucket = getBucket(xloc, yloc, mapx, mapy, columns, rows);
	std::cout << " Current Bucket: " << currentBucket << std::endl;

	int x = currentBucket / rows;
	int y = currentBucket % rows;
	int xval;
	int yval;

	//Algorithm for finding the surrounding relative buckets while taking the edges of the map into account
	//It converts the buckets into a basic coordinate system
	for (int i = rad; i >= 0; i--){
		for (int j = rad; j >= 0; j--){

			xval = x - i;
			yval = y - j;

			if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
				surroundings.push_back(backToBuck(xval, yval, rows));
			}
		}
		for (int j = 1; j <= rad; j++){

			xval = x - i;
			yval = y + j;

			if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
				surroundings.push_back(backToBuck(xval, yval, rows));
			}
		}
	}
	for (int i = 1; i <= rad; i++){
		for (int j = rad; j >= 0; j--){

			xval = x + i;
			yval = y - j;

			if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
				surroundings.push_back(backToBuck(xval, yval, rows));
			}
		}
		for (int j = 1; j <= rad; j++){

			xval = x + i;
			yval = y + j;

			if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
				surroundings.push_back(backToBuck(xval, yval, rows));
			}
		}
	}

	int thresh = surroundings.size(); //The following is for testing only
	std::cout << " Size: " << surroundings.size() << std::endl;
	for (int k = 0; k < thresh; k++){
		std::cout << " [" << k << "] = " << surroundings[k] << std::endl;
	}

	return surroundings;
}

//Meant for use in the Surroudings function to re-convert coordinates to buckets
template <class T>
int HMBL<T>::backToBuck(int x, int y, int ys){
	int cB = (x*ys) + y;
	return cB;
}

int main(int argc, const char* argv[]) //Only used for testing
{
	/*printf("Begin Testing of HMBL\n\nRound One:\n");
	HMBL* testHMBL = new HMBL(200, 200, 20, 20);
	printf(" Dimensions: %d, %d\n", testHMBL->mapheight, testHMBL->mapwidth);
	printf(" Bucket x = %d, y = %d\n\n", testHMBL->columns, testHMBL->rows);
	printf(" Where are you at 72,36? Bucket #%d\n", testHMBL->findBucket(72, 36));
	printf(" How about 200,200? Bucket #%d\n", testHMBL->findBucket(200, 200));
	printf("\nRound Two:\n");
	testHMBL->update(7200, 213, 72, 36);
	testHMBL->update(7300, 12, 56, 150);
	testHMBL->update(7400, 54, 56, 150);
	testHMBL->update(8800, 72, 56, 150);
	testHMBL->update(7400, 54, 100, 100);
	testHMBL->revealHMBL();
	printf("\nRound Three:\n");
	testHMBL->surroundings(4, 4, 2, 100, 100, 20, 20);

	getchar();*/
}
