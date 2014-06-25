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


    const int BUFSIZE = 1 << 10;

    typedef string UserId; // EUID
    typedef unsigned int ChatId;
    typedef unsigned short SubServerId;

    struct UserInfo {
	UserId id;
	string name; // User name shown on client side
	bool isOnline; // If user is currently online
	sockaddr_in addr; // User address, including ip, port and other data
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
	
	string jsonString();
    
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

    string Chat::jsonString() {
	string json("{");
	json += "name: \"chat\",";
	json += "id:";
	json += std::to_string(_id);
	json += ", users:[";
	vector<UserId>::iterator it = _userList.begin();
	json += *it;
	it ++;
	while(it != _userList.end()) {
	    json += ",";
	    json += *it;
	    it ++;
	}
	json += "]}";

	return json;	    
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
