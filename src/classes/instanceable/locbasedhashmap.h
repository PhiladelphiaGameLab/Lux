#include <mutex>

template <class T>
struct Node
{
	T sock; //Client Socket
	int lastBuck; //Position in arrMap of the last bucket to contain this node
	int euid; //End user ID
	struct Node *Next; //Pointer to the next node if this is in a linked list of nodes
	struct Node *Prev; //Pointer to the previous noe if this is in a linked list of nodes

	// TODO: My attepmt at a lock (Paul -- used in sendUpdate.cpp)
	// TODO: need to initilize the lock still...maybe something like this: pthread_mutex_init(Lock, NULL)
	pthread_mutex_t *Lock;
};

//Nodes that handle hashmap collisions.
struct CNode 
{
	struct Node* Base;
	struct CNode *Next;
	struct CNode *Prev;
};


// Added struct for bucket to be called from sendUpdate.cpp (Paul & Sibi)
struct Bucket
{
	struct Node *Head;
	Bucket *Next;
};


template <class T>
class HMBL{
private:
	int mapwidth;
	int mapheight;
	int columns;
	int rows;
	int bucketTotal;
	Node* arrMap[];
	CNode* hashTable[];

public:
	HMBL(int mapw, int maph, int col, int row);
	HMBL();
	int findBucket(int x, int y);
	void update(T nsock, int euid, int x, int y);
	Node checkForCollision(int euid, int hashKey);
	void clearHMBL();

};

