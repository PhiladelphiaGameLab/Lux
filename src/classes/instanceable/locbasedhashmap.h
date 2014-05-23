
template <class T>
struct Node
{
	T sock; //Client Socket
	int lastBuck; //Position in arrMap of the last bucket to contain this node
	struct Node *Next; //Pointer to the next node if this is in a linked list of nodes
	struct Node *Prev; //Pointer to the previous noe if this is in a linked list of nodes
};


template <class T>
class HMBL{
	private:
		int mapwidth;
		int mapheight;
		int columns;
		int rows;
		int bucketTotal;
		int* arrMap[];
		Node* hashTable[];
		struct Node *head;
	public:
		HMBL(int mapw, int maph, int col, int row);
		HMBL();
		int findBucket(int x, int y)
};