#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "luxSocket.h"
#include <cstring>
#include <string>
#include <vector>
#include <map>

namespace chat{

    using std::string;
    using std::vector;
    using std::pair;
    using std::map;


    // Maximum size of UDP packet
    // 0xffff - (sizeof(IP Header) + sizeof(UDP Header)) = 65535-(20+8) = 65507
    const int BUFSIZE = 65507;

    typedef string UserId; // EUID
    typedef unsigned int ChatId;
    typedef unsigned short SubServerId;
    typedef unsigned int MsgId;
    typedef unsigned char BYTE;

    struct UserInfo {
	UserId id;
	bool isOnline; // If user is currently online
	sockaddr_in addr; // User address, including ip, port and other data
	sockaddr_in pollAddr;
    };

    class Chat {
	public:
    
	Chat(int capacity = 100) : _capacity(capacity) {
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
	};
    
	void insertUser(UserId item) {
	    _userList.push_back(item);
	};

	void setAddress(const string &addr) {
	    _address = addr;
	};
    
	void setPortNum(unsigned short port) {
	    _portNum = port;
	};
	
	string toString();
    
	private:
	string _address;
	unsigned short _portNum;
	ChatId _id;
	int _capacity;
	vector<UserId> _userList;

	static ChatId getNewId() {
	    static ChatId id = 0;
	    return id++;
	};
    };

    string Chat::toString() {
	string str("");
	for (int i = 0; i < sizeof(_id); i++) {
	    str.push_back(*((char*)&_id + i));
	}
	for (int i = 0; i < sizeof(_capacity); i++) {
	    str.push_back(*((char*)&_capacity + i));
	}
	int size = _userList.size();
	for (int i = 0; i < sizeof(size); i++) {
	    str.push_back(*((char*)&size + i));
	}
	
	for (vector<UserId>::iterator it = _userList.begin();
	     it != _userList.end();
	     it ++) {
	    str += *it;
	}
	return str;
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

	socketlibrary::LuxSocket& getSocket() {
	    return *_udpSocket;
	};
    
	string getAddress() {
	    return _udpSocket->getAddress();
	};
    
	unsigned short getPortNum() {
	    return _udpSocket->getPortNum();
	};

	string receive();

	private:
	static const unsigned short _startPort = 10000;
	socketlibrary::LuxSocket *_udpSocket;
	SubServerId _id;
	int _capacity;
	map<ChatId, Chat*> _chatPool;
	static SubServerId getNewId() {
	    static SubServerId id = 0;
	    return id++;
	};
    };
}

#endif // CHATSERVER_H
