#ifndef HMBL_H
#define HMBL_H

#include <mutex>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>


#include "SendNewRelevant.h"

template <class T>
struct Node
{
	T sock; //Client Socket
	int currBuck; //Position in arrMap of the last bucket to contain this node
	int euid; //End user ID
	struct Node<T> *Next; //Pointer to the next node if this is in a linked list of nodes
	struct Node<T> *Prev; //Pointer to the previous noe if this is in a linked list of nodes

	// TODO: My attepmt at a lock (Paul -- used in sendUpdate.cpp)
	// TODO: need to initilize the lock still...maybe something like this: pthread_mutex_init(Lock, NULL)
	pthread_mutex_t *Lock;

};

//Nodes that handle hashmap collisions.
template <class T>
struct CNode
{
	struct Node<T>* Base;
	struct CNode<T> *Next;
	struct CNode<T> *Prev;
};


// Added struct for bucket to be called from sendUpdate.cpp (Paul & Sibi)i
template <class T>
struct Bucket
{
	struct Node<T> *Head;
	Bucket<T> *Next;
};


template <class T>
class HMBL{
private:
	/*int mapwidth;
	int mapheight;
	int columns;
	int rows;
	int bucketTotal;
	Node* arrMap[400];
	CNode* hashTable[200];*/

public:
	//For Testing Purposes:
	int mapwidth;
	int mapheight;
	int columns;
	int rows;
	int bucketTotal;
	int pipeFD;
	Node<T>** arrMap;
	CNode<T>** hashTable;

	HMBL(int mapw, int maph, int col, int row, std::string pipeLocation);
	HMBL();
	int findBucket(int x, int y);
	void update(T nsock, int euid, int x, int y, int rad);
	void clearHMBL();
	void revealHMBL();
	void pipeInfo(int x, int y, int rad, int currBuck);
	Node<T>* checkForCollision(int euid, int hashKey);
	std::vector<Node<T>*> get_clients(int xloc, int yloc, int rad);
	std::vector<int> surroundingsFromCurrBucket(int currBuck, int rad, int mapx, int mapy, int xbuck, int ybuck);
};


class HMBL_HELPER {

public:
	static int getBucket(int xloc, int yloc, int mapx, int mapy, int columns, int rows);
	static std::vector<int> surroundings(int xloc, int yloc, int rad, int mapx, int mapy, int xbuck, int ybuck);
	static int backToBuck(int x, int y, int ys);
};


/**********  The Double Linked List Struct (for reference) *********/
//struct CNode
//{struct Node
//{
//	T sock;
//	int currBuck = 0;
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
	return HMBL_HELPER::getBucket(x, y, mapwidth, mapheight, columns, rows);
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
			//hashTable[i]->Base->sock = 0;
			hashTable[i]->Base->euid = 0;
			hashTable[i]->Base->Next = 0;
			hashTable[i]->Base->Prev = 0;
			hashTable[i]->Base->currBuck = -1;
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
				//std::cout << " hashTable[" << i << "] contains user " << hashTable[i]->Base->euid << std::endl;
				//std::cout << " " << hashTable[i]->Base->currBuck << " " << hashTable[i]->Base->sock << " Prev:" << hashTable[i]->Base->Prev << " Next:" << hashTable[i]->Base->Next << std::endl;
			}
		}
	}
}

template <class T>
std::vector<Node<T>*> HMBL<T>::get_clients(int xloc, int yloc, int rad){
	std::vector<Node<T>*> clients; //will be returned by this function and contain pointers to affected clients
	std::vector<int> proxBucks; //contains the list of buckets in the items proximity

	 std::cout<<"Entering surroundings in  get_clients"<<std::endl;
	proxBucks = HMBL_HELPER::surroundings(xloc, yloc, rad, mapwidth, mapheight, columns, rows);

	int thresh = proxBucks.size();
	Node<T>* walk;
	int ctr = 0;
	std::cout<<"Returned bucket size:"<<thresh<<std::endl;
	for (int i = 0; i < thresh; i++){
		walk = arrMap[proxBucks[i]];
		ctr = 0;
		//std::cout<<"Value inside for loop i:"<<i<<std::endl;
		while (walk != 0){
		ctr++;
		cout<< "walk in HMBL is : "<<walk <<endl;
		 //std::cout<<"Value inside while loop ctr:"<<ctr<<std::endl;
			clients.push_back(walk);
			walk = walk->Next;
		}
	}
	std::cout<<"Returned Successfully from get_clients"<<std::endl;
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
					surroundings.push_back(HMBL_HELPER::backToBuck(xval, yval, rows));
				}
			}
			for (int j = 1; j <= rad; j++){

				xval = x - i;
				yval = y + j;

				if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
					surroundings.push_back(HMBL_HELPER::backToBuck(xval, yval, rows));
				}
			}
		}
		for (int i = 1; i <= rad; i++){
			for (int j = rad; j >= 0; j--){

				xval = x + i;
				yval = y - j;

				if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
					surroundings.push_back(HMBL_HELPER::backToBuck(xval, yval, rows));
				}
			}
			for (int j = 1; j <= rad; j++){

				xval = x + i;
				yval = y + j;

				if (xval < columns && xval >= 0 && yval < rows && yval >= 0){
					surroundings.push_back(HMBL_HELPER::backToBuck(xval, yval, rows));
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
	std::vector<int> newSurr = HMBL_HELPER::surroundings(x, y, rad, mapwidth, mapheight, columns, rows);

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
	newSurrBuck->newBucketList = impSurr;
	//newSurrBuck->socket = ;
	cout<<"HMBL trying to pipe to SNR"<<endl;
	write(pipeFD, newSurrBuck, sizeof(s_SNRMessage));
	cout<<"HMBL piped to SNR"<<endl;

}


#endif
