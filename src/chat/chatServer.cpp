#include "luxSocket.h"
#include <cstring>
#include <string>
#include <vector>
#include <map>

using std::string;
using std::map;
using std::pair;
using std::vector;

static const int BUFSIZE = 1 << 10;

typedef int UserId; // Can be changed later to something like EUID
typedef short ChatId;

struct UserInfo {
    UserId id;
    string name; // User name shown on client side
    bool isOnline; // If user is currently online
    sockaddr_in addr; // User address, including ip, port and other data
};

// Structure for each chat, contains chat id and users in the chat
struct ChatInfo {
    ChatId id;
    vector<UserId> userList; // Stores user id instead of full user information.
};

// Stores all online user information here
map<UserId, UserInfo *> userPool;

// Stores all chats in the list
map<ChatId, ChatInfo *> chatPool;

// Socket
socketlibrary::LuxSocket *udpSocket;

// Creates chat for users in the list
// Sends back the chat data so that all clients and create coressponding chat box
void createChat(vector<UserId> idList);

// Make chat info into a string
void makeChatInfo(ChatId chatId, string &msgChatInfo);

// Send message to a client
void sendMessage(UserId userId, const string &msg);

// Sends message to all users expect the sender in this chat
// Need to confirm with  sender
void sendMessage(UserId userId, ChatId chatId, const string &msg);

// Update chats in chat list
// Check if users are still online, if not remove from list
// Check if chat is still valid
void updateChatPool();

// Update users in user pool
// Gets user information from social network server and updates users status
void updateUserPool();

// Helper functions

// Returns a new chat id
ChatId getNewChatId();
string toString(UserId userId);
string toString(ChatId chatId);
bool equalId(UserId id0, UserId id1);

int main() {
    unsigned short port = 8000;
    udpSocket = new socketlibrary::LuxSocket(port);
    while (1) {
	// Receive data from clients and do whatever need to do.
    }
}

void createChat(vector<UserId> idList) {
    ChatInfo *chat = new ChatInfo();
    
    chat->id = getNewChatId();
    
    for (vector<UserId>::iterator it = idList.begin();
	 it != idList.end();
	 it ++) {
	chat->userList.push_back(*it);
    }
    
    // Insert new chat into chat pool
    chatPool.insert(pair<ChatId, ChatInfo *>(chat->id, chat));

    // Send message to all clients
    string msg("{");
    msg += "status: createNewChat, id:\"";
    msg += toString(chat->id);
    msg += "\"}";
    for (vector<UserId>::iterator it = chat->userList.begin();
	 it != chat->userList.end();
	 it ++) {
	sendMessage(*it, msg);
    }
}

void makeChatInfo(ChatId chatId, string &msgChatInfo) {
    string str("{status:");
    map<ChatId, ChatInfo *>::iterator chatMapIt = chatPool.find(chatId);

    // Chat not found
    if (chatMapIt == chatPool.end()) {
	str += "error, message:\"Chat doesn't exist.\"}";
    }
    else {
	ChatInfo *chat = chatMapIt->second;

	str += "chatInfo, id: \"";
	str += toString(chat->id);
	str += "\", userList:[";
	vector<UserId>::iterator usrListIt = chat->userList.begin();
	str += toString(*usrListIt);
	for (; usrListIt != chat->userList.end(); usrListIt++) {
	    str += ",";
	    str += toString(*usrListIt);
	}
	str += "]}";
    }
    msgChatInfo = str;
}

void sendMessage(UserId userId, const string &msg) {
    map<UserId, UserInfo *>::iterator usrMapIt = userPool.find(userId);
    
    // User not found
    if (usrMapIt != userPool.end()) {
	UserInfo *user = usrMapIt->second;
	udpSocket->send(msg, &user->addr);    
    }
}

void sendMessage(UserId userId, ChatId chatId, const string &msg) {
    // Find chat
    map<ChatId, ChatInfo *>::iterator chatMapIt = chatPool.find(chatId);
    if (chatMapIt != chatPool.end()) {
	ChatInfo *chat = chatMapIt->second;
	// Find each user id
	for (vector<UserId>::iterator usrVecIt = chat->userList.begin();
	     usrVecIt != chat->userList.end();
	     usrVecIt ++) {
	    if (!equalId(*usrVecIt, userId)) {
		// Send message to other users
		sendMessage(*usrVecIt, msg);
	    }
	}
    }
}

void updateChatPool() {
    map<ChatId, ChatInfo *>::iterator chatMapIt = chatPool.begin();
    // Iterating all chats 
    while (chatMapIt != chatPool.end()) {
	// Check all users in each chat
	ChatInfo *chat = chatMapIt->second;
	vector<UserId>::iterator usrVecIt = chat->userList.begin();
	while(usrVecIt != chat->userList.end()) {
	    map<UserId, UserInfo *>::iterator usrMapIt = userPool.find(*usrVecIt);
	    if (usrMapIt == userPool.end() || !(usrMapIt->second)->isOnline) {
		// User not in pool or user is not online
		chat->userList.erase(usrVecIt);
	    }
	    else {
		usrVecIt ++;
	    }	    
	}
	if (chat->userList.size() == 0) {
	    // If no one is in chat, remove chat
	    delete (chatMapIt->second);
	    chatPool.erase(chatMapIt);
	}
	else {
	    chatMapIt ++;
	}
    }
}

void updateUserPool() {
    vector<UserInfo> changed;
    // Get user information from social net work server
    // Assume all changed users infomation stored in vector<UserInfo> changed
    
    for (vector<UserInfo>::iterator it = changed.begin();
	 it != changed.end();
	 it ++) {
	UserId id = (*it).id;
	map<UserId, UserInfo *>::iterator usrMapIt = userPool.find(id);	
	if (usrMapIt != userPool.end()) {
	    // User already in pool
	    UserInfo *user = usrMapIt->second;
	    user->name = (*it).name;
	    user->isOnline = (*it).isOnline;
	    user->addr = (*it).addr;	 
	}
	else {
	    // New user
	    UserInfo *user = new UserInfo();
	    user->id = (*it).id;
	    user->name = (*it).name;
	    user->isOnline = (*it).isOnline;
	    user->addr = (*it).addr;
	    userPool.insert(pair<UserId, UserInfo *>(user->id,user));
	}
    }    
}

ChatId getNewChatId() {
    static ChatId id = 0;
    return id++;
}

string toString(UserId userId) {
    return std::to_string(userId);
}

string toString(ChatId chatId) {
    return std::to_string(chatId);
}

bool equalId(UserId id0, UserId id1) {
    return id0 == id1;
}
