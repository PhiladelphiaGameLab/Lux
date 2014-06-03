#include <mutex>
#include <vector>

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
	//pthread_mutex_t *Lock;

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

//Struct for pipe to SNR Thread
typedef struct s_SNR
{
	const char *pipeLocation;
};

typedef struct s_SNRMessage
{
	std::vector<int> newBuckList;
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
	Node** arrMap;
	CNode** hashTable;

	HMBL(int mapw, int maph, int col, int row);
	HMBL();
	int findBucket(int x, int y);
	void update(T nsock, int euid, int x, int y, int rad);
	void clearHMBL();
	void revealHMBL();
	void pipeInfo(int x, int y, int rad, int lastBuck);
	Node* checkForCollision(int euid, int hashKey);
	std::vector<Node*> get_clients(int xloc, int yloc, int rad);
	std::vector<int> surroundingsFromCurrBucket(int currBuck, int rad, int mapx, int mapy, int xbuck, int ybuck);

	static int getBucket(int xloc, int yloc, int mapx, int mapy, int columns, int rows);
	static std::vector<int> surroundings(int xloc, int yloc, int rad, int mapx, int mapy, int xbuck, int ybuck);
	static int backToBuck(int x, int y, int ys);

};

