#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "chatUtility.h"
#include "luxSocket.h"
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <boost/thread/thread.hpp>

namespace chat{

    using std::string;
    using std::vector;
    using std::pair;
    using std::map;

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

	ChatId getId() {
	    return _id;
	};
    
	int getCapacity() {
	    return _capacity;
	};
    
	bool isEmpty() {
	    return _userList.size() == 0;
	};
    
	int emptySpace() {
	    return _capacity - _userList.size();
	};

        vector<UserId>& getList() {
	    return _userList;
	};

	void eraseUser(typename vector<UserId>::iterator &it) {
	    _userList.erase(it);
	    _changed = true;
	};
    
	void insertUser(UserId item) {
	    _userList.push_back(item);
	    _changed = true;
	};

	void setAddress(const string &addr) {
	    _address = addr;
	    _changed = true;
	};
    
	void setPortNum(unsigned short port) {
	    _portNum = port;
	    _changed = true;
	};
	
	const vector<BYTE>& toBytes();

	private:
	string _address;
	unsigned short _portNum;
	ChatId _id;
	int _capacity;
	vector<UserId> _userList;
	vector<BYTE> bytes;
	bool _changed;

	static ChatId getNewId() {
	    static ChatId id = 0;
	    return id++;
	};
    };

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

	int size = _userList.size();
	for (int i = 0; i < sizeof(size); i++) {
	    bytes.push_back(*((BYTE*)&size + i));
	}

	for (vector<UserId>::iterator it = _userList.begin();
	     it != _userList.end();
	     it ++) {
	    for (int i = 0; i < (*it).length(); i++) {
		bytes.push_back((*it)[i]);
	    }
	    std::cout << (*it) << std::endl;
	}

	return bytes;
    }

    class SubServer {
	public:
	SubServer(int capacity = 100) : _capacity(capacity) {
	    _id = getNewId();
	    _udpSocket = new socketlibrary::LuxSocket(_id + _startPort);
	};
        
	~SubServer() {
	    delete _udpSocket;
	};
    
	SubServerId getId() {
	    return _id;
	};
    
	int getCapacity() {
	    return _capacity;
	};
    
	bool isEmpty() {
	    return _chatPool.size() == 0;
	};
    
	int emptySpace() {
	    return _capacity - _chatPool.size();
	};
	
	Chat* getChat(ChatId id) {
	    map<ChatId, Chat*>::iterator it = _chatPool.find(id);
	    if (it != _chatPool.end()) {
		return it->second;
	    }
	    return NULL;
	};

	map<ChatId, Chat*>& getChats() {
	    return _chatPool;
	};

	void eraseChat(typename map<ChatId, Chat*>::iterator &it) {
	    _chatPool.erase(it);
	};
    
	void inserChat(Chat &chat) {
	    _chatPool.insert(pair<ChatId, Chat*>(chat.getId(), &chat));
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

	private:
	static const unsigned short _startPort = 30000;
	socketlibrary::LuxSocket *_udpSocket;
	SubServerId _id;
	int _capacity;
	map<ChatId, Chat*> _chatPool;
	static SubServerId getNewId() {
	    static SubServerId id = 0;
	    return id++;
	};
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

}

#endif // CHATSERVER_H
