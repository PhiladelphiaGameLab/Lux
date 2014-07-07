#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "chatUtility.h"
#include "luxSocket.h"
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <boost/thread/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <mutex>

namespace chat{

    using std::string;
    using std::vector;
    using std::pair;
    using std::map;
    using socketlibrary::LuxSocket;
    using boost::thread;

    struct UserInfo {
	UserId id;
	bool isOnline; // If user is currently online
	sockaddr_in addr; // User address, including ip, port and other data
	sockaddr_in pollAddr;
    };

    class Chat {
	public:
    
	Chat(int capacity = 100) : _capacity(capacity), _changed(true) {
	    _id = getNewId();
	};

	ChatId getId() const {
	    return _id;
	};
    
	int getCapacity() const {
	    return _capacity;
	};
    
	bool isEmpty() const {
	    return _userNum == 0;
	};
    
	int emptySpace() const {
	    return _capacity - _userNum;
	};
	
        const list<UserId>& getList() {
	    return _userList;
	};

	void eraseUser(const typename list<UserId>::const_iterator &it) {
	    _userList.erase(it);
	    _userNum = _userList.size();
	    _changed = true;
	};
    
	void insertUser(const vector<UserId> &idArray);

	void setAddress(const string &addr) {
	    _address = addr;
	    _changed = true;
	};
    
	void setPortNum(unsigned short port) {
	    _portNum = port;
	    _changed = true;
	};
	
	const vector<BYTE>& toBytes();

	boost::shared_mutex& getReaderMutex() {
	    return _userListMutex;
	}

	bool quitChat(const UserId &id, MESSAGE_TYPE &msgType);

	private:
	string _address;
	unsigned short _portNum;
	ChatId _id;
	int _capacity;
	int _userNum;
	list<UserId> _userList;
	vector<BYTE> bytes;
	bool _changed;
	// Reader and writer lock for _userList
	boost::shared_mutex _userListMutex;

	static ChatId getNewId() {
	    static ChatId id = 0;
	    return id++;
	};
    };

    void Chat::insertUser(const vector<UserId> &idArray) {
	// writer 
	// get upgrade access
	boost::upgrade_lock<boost::shared_mutex> lock(_userListMutex);
	// get exclusive access
	boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
	    
	for (vector<UserId>::const_iterator it = idArray.begin();
	     it != idArray.end();
	     it ++) {
	    _userList.push_back(*it);
	}
	_userNum = _userList.size();
	_changed = true;
    }

    const vector<BYTE>& Chat::toBytes() {
	if (!_changed)
	    return bytes;
	bytes.clear();
	
	for (int i = 0; i < sizeof(_portNum); i++) {
	    bytes.push_back(*((BYTE*)&_portNum + i));
	}

	for (int i = 0; i < sizeof(_id); i++) {
	    bytes.push_back(*((BYTE*)&_id + i));
	}

	for (int i = 0; i < sizeof(_capacity); i++) {
	    bytes.push_back(*((BYTE*)&_capacity + i));
	}

	// reader
	// get upgrade access
	boost::upgrade_lock<boost::shared_mutex> lock(_userListMutex);

	int size = _userList.size();
	for (int i = 0; i < sizeof(size); i++) {
	    bytes.push_back(*((BYTE*)&size + i));
	}
		
	for (list<UserId>::iterator it = _userList.begin();
	     it != _userList.end();
	     it ++) {
	    for (int i = 0; i < (*it).length(); i++) {
		bytes.push_back((*it)[i]);
	    }
	    std::cout << (*it) << std::endl;
	}

	return bytes;
    }

    bool Chat::quitChat(const UserId &id, MESSAGE_TYPE &msgType) {
	// writer 
	// get upgrade access
	boost::upgrade_lock<boost::shared_mutex> lock(_userListMutex);
	// get exclusive access
	boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
	    
	for (list<UserId>::const_iterator it = _userList.begin();
	     it != _userList.end();
	     it ++) {
	    if (id == *it) {
		eraseUser(it);
		msgType = CONFIRM;
		return true;
	    }
	}	    
	return false;
    }



    class SubServer {
	public:
	SubServer(int capacity = 100) : _capacity(capacity), _chatNum(0) {
	    // Assign port 0, let OS to find an open port
	    try {
		_udpSocket = new LuxSocket(0);
	    }
	    catch (string e){
		throw e;
	    }
	};
        
	~SubServer() {
	    if (_udpSocket) {
		delete _udpSocket;
	    }
	    if (_thisThread) {
		_thisThread->interrupt();
		if (_thisThread) {
		    delete _thisThread;
		}
	    }
	    for (map<ChatId, Chat*>::iterator it = _chatPool.begin();
		 it != _chatPool.end();
		 it ++) {
		if (it->second) {
		    delete it->second;
		}
	    }	    
	};
    
	int getCapacity() {
	    return _capacity;
	};
    
	bool isEmpty() {
	    return _chatNum == 0;
	};
    
	int emptySpace() {
	    return _capacity - _chatNum;
	};
	
	Chat* getChat(ChatId id) {
	    // reader
	    // get upgrade access
	    boost::upgrade_lock<boost::shared_mutex> lock(_chatPoolMutex);
	    
	    map<ChatId, Chat*>::iterator it = _chatPool.find(id);
	    if (it != _chatPool.end()) {
		return it->second;
	    }
	    return NULL;
	};

	map<ChatId, Chat*>& getChats() {
	    return _chatPool;
	};

	void eraseChat(const typename map<ChatId, Chat*>::iterator &it) {
	    _chatPool.erase(it);
	    _chatNum--;
	};
    
	void insertChat(Chat &chat) {
	    // write
	    // get upgrade access
	    boost::upgrade_lock<boost::shared_mutex> lock(_chatPoolMutex);
	    // get exclusive access
	    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
	    
	    _chatPool.insert(pair<ChatId, Chat*>(chat.getId(), &chat));
	    _chatNum++;
	};

	socketlibrary::LuxSocket* getSocket() {
	    return _udpSocket;
	};
    
	string getAddress() {
	    return _udpSocket->getAddress();
	};
    
	unsigned short getPortNum() {
	    return _udpSocket->getPortNum();
	};

	string receive();
	
	void setThread(boost::thread *t) {
	    _thisThread = t;
	}
	
	boost::shared_mutex& getChatPoolMutex() {
	    return _chatPoolMutex;
	}

	private:
	boost::thread *_thisThread;
	socketlibrary::LuxSocket *_udpSocket;
	int _capacity;
	int _chatNum;
	map<ChatId, Chat*> _chatPool;
	// Reader and writer lock for _chatPool
	boost::shared_mutex _chatPoolMutex;
    };
    
    // Thread manager class
    // When expires, it will kill all child threads
    class ThreadMgr : public std::vector<boost::thread*> {
	public:
	~ThreadMgr() {
	    for (std::vector<boost::thread*>::iterator it = begin();
		 it != end();
		 it ++) {
		if ((*it) != NULL) {
		    (*it)->interrupt();
		}
	    }

	};
    };

    class ChatServer {
	public:
	ChatServer(unsigned short port = 3000) 
	    : _updatingUserPool(false), _updatingChats(false) {
	    _mainSock = new LuxSocket(port);
	};
	~ChatServer();
	void run();
	
	private:
	// Stores all online user information here
	map<UserId, UserInfo*> _userPool;
	// Reader/Writer mutex for _userPool
	boost::shared_mutex _userPoolMutex;
	// Indicate whether _userPool is been updating
	bool _updatingUserPool;

	// Stores all chats in the list
	list<SubServer *> _subServerList;
	// Exclusive Mutex for sub server list
	std::mutex _subServerMutex;
	// Indicate whether _userPool is been updating
	bool _updatingChats;

	LuxSocket *_mainSock;

	// Time between two calls of update
	int _updateTime = 10;

	
	// Server functions
	
	// Add new user into user pool
	UserInfo* connect(UserId &id, sockaddr_in &addr, unsigned short port, 
			  unsigned short pollPort);

	// Clear user online status
	bool disconnect(UserInfo &user);

	// Creates chat for users
	Chat* createChat(const UserInfo &user, const vector<UserId> &idArray, 
			 MESSAGE_TYPE &msgType);
	void addUserToChat(Chat &chat, const vector<UserId> &idArray,
			   MESSAGE_TYPE &msgType);
	void quitChat(UserInfo &user, Chat &chat, MESSAGE_TYPE &msgType);


	// Find a sub server for chat 
	SubServer* findSubServer();

	// Create a new sub server
	SubServer* createNewSubServer();

	void sendToAll(BYTE *buf, size_t len, LuxSocket *sock, Chat &chat);
	// Sends message to all users expect the sender in this chat
	void sendToOthers(BYTE *buf, size_t len, LuxSocket *sock, Chat &chat, 
			  UserId &senderId);

	// Threads to handle requests
	void mainRequestHandler(BYTE *buf, size_t len, sockaddr_in *tmpAddr);
	void chatRequestHandler(BYTE *buf, size_t len, sockaddr_in *tmpAddr, 
				SubServer *serv);

	// Sub server thread
	void startSubServerThread(SubServer *serv);
	
	
	// Helper functions
	UserInfo* findUser(UserId id);
	bool verifyUser(UserInfo *userPtr, sockaddr_in &cliAddr);
	int sockAddrCmp(const sockaddr_in &a, const sockaddr_in &b);

	// Update functions
	void updateAll();
	void updateUserPool();	
	void updateUserPorts(UserInfo &user, unsigned short recvPort, 
			     unsigned short pollPort);
	void updateSubServer();
	void updateChats(SubServer &subServ);
	
	

	static bool equalId(const UserId &id0, const UserId &id1);
    };
    

    // Set a boolean to false when it expires
    class IndicatorHelper {
	public:
	IndicatorHelper(bool &i) : _indicator(i) {
	    _indicator = true;
	};
	~IndicatorHelper() {
	    _indicator = false;
	};
	private:
	bool _indicator;
    };
}

#endif // CHATSERVER_H
