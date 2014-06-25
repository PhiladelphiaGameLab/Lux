#include "chatServer.h"

using namespace chat;
using socketlibrary::LuxSocket;
// Stores all online user information here
map<UserId, UserInfo *> userPool;

// Stores all chats in the list
list<SubServer *> subServList;

// Socket
LuxSocket *udpServer;

// Creates chat for users in the list
// Sends back the chat data so that all clients and create coressponding chat box
void createChat(vector<UserId> idList);

// Find a sub server for chat 
SubServer* findSubServer();

// Create a new sub server
SubServer* createNewSubServer();

// Make chat info into a string
void makeChatInfo(ChatId chatId, string &msgChatInfo);

// Send message to a client
void sendMessage(LuxSocket &sock, const UserId &userId,
		 const string &msg);

// Sends message to all users expect the sender in this chat
// Need to confirm with  sender
void sendMessage(SubServer &serv, const UserId &userId, 
		 ChatId chatId, const string &msg);

// Update chats in chat list
// Check if users are still online, if not remove from list
// Check if chat is still valid
void updateChatPool();

// Update users in user pool
// Gets user information from social network server and updates users status
void updateUserPool();
    
// Add new user into user pool
void addNewUser(UserInfo *user);

// Sub server thread
void *startSubServerThread(void *vargp);

// Helper functions

// Returns a new chat id
ChatId getNewChatId();
string toString(UserId userId);
string toString(ChatId chatId);
bool equalId(UserId id0, UserId id1);

int main() {
    unsigned short port = 8000;
    udpServer = new LuxSocket(port);
    while (1) {
	// This is the main server.
	// Receive data from clients and do whatever need to do.
	// Will create new chats and assign them to sub servers and 
	// keep updating all users information.
    }
    return 0;
}

void *startSubServerThread(void *vargp) {
    SubServer *serv = (SubServer*)vargp;
    LuxSocket sock = serv->getSocket();
	
    struct sockaddr_in cliAddr;	
    char *buf = new char[BUFSIZE];
	
    while (1) {	    
	sock.receive(buf, &cliAddr);
	    
	// Receive message from client 
	// Do whatever need to do
    }

    return NULL;
}

void createChat(vector<UserId> idList) {
    SubServer* serv = findSubServer();
    if (serv == NULL) {
	// No more server is available
	return;
    }
    
    Chat *chat = new Chat();
    chat->setAddress(serv->getAddress());
    chat->setPortNum(serv->getPortNum());

    
    for (vector<UserId>::iterator it = idList.begin();
	 it != idList.end();
	 it ++) {
	chat->insertUser(*it);
    }
    
    // Insert new chat into sub server
    serv->inserChat(*chat);
    
    // Send message to all clients
    string msg("{");
    msg += "status: createNewChat, message:";
    msg += chat->jsonString();
    msg += "}";
    for (vector<UserId>::iterator it = chat->getList().begin();
	 it != chat->getList().end();
	 it ++) {
	sendMessage(*udpServer, *it, msg);
    }
}

SubServer* findSubServer() {
    // Find a suitable sub server
    list<SubServer *>::iterator it;
    for (it = subServList.begin(); it != subServList.end(); it ++) {
	if ((*it)->emptySpace() >= 1) {
	    break;
	}
    }
    
    SubServer *server = NULL;
    
    if (it != subServList.end()) {
	// Found one
	server = *it;
    }
    else {
	// Didn't found, carete a new server
	server = createNewSubServer();
	subServList.push_front(server);
    }
    return server;
}


SubServer* createNewSubServer() {
    SubServer *server = new SubServer();
    pthread_t tid;
    // Start sub server thread
    pthread_create(&tid, NULL, startSubServerThread, server);
    return server;
}

void sendMessage(LuxSocket &sock, const UserId &userId, 
		 const string &msg) {
    map<UserId, UserInfo *>::iterator usrMapIt = userPool.find(userId);
    
    // User not found
    if (usrMapIt != userPool.end()) {
	UserInfo *user = usrMapIt->second;
	sock.send(msg, &(user->addr));
    }
}

void sendMessage(SubServer &serv, const UserId &userId, 
		 ChatId chatId, const string &msg) {
    map<ChatId, Chat*> chatPool = serv.getChats();
    // Find chat
    map<ChatId, Chat*>::iterator chatMapIt = chatPool.find(chatId);
    if (chatMapIt != chatPool.end()) {
	Chat *chat = chatMapIt->second;
	// Find each user id
	for (vector<UserId>::iterator usrVecIt = chat->getList().begin();
	     usrVecIt != chat->getList().end();
	     usrVecIt ++) {
	    if (!equalId(*usrVecIt, userId)) {
		// Send message to other users
		sendMessage(serv.getSocket(), *usrVecIt, msg);
	    }
	}
    }
}


void makeChatInfo(ChatId chatId, string &msgChatInfo) {
	
}

void updateChatPool(SubServer &subServ) {
    map<ChatId, Chat*> chatPool = subServ.getChats();
    map<ChatId, Chat*>::iterator chatMapIt = chatPool.begin();
    // Iterating all chats 
    while (chatMapIt != chatPool.end()) {
	// Check all users in each chat
	Chat *chat = chatMapIt->second;
	vector<UserId>::iterator usrVecIt = chat->getList().begin();
	while(usrVecIt != chat->getList().end()) {
	    map<UserId, UserInfo *>::iterator usrMapIt = userPool.find(*usrVecIt);
	    if (usrMapIt == userPool.end() || !(usrMapIt->second)->isOnline) {
		// User not in pool or user is not online
		chat->eraseUser(usrVecIt);
	    }
	    else {
		usrVecIt ++;
	    }	    
	}
	if (chat->isEmpty()) {
	    // If no one is in chat, remove chat
	    delete (chatMapIt->second);
	    subServ.eraseChat(chatMapIt);
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
	    userPool.insert(pair<UserId, UserInfo*>(user->id,user));
	}
    }    
}
    
void addNewUser(UserInfo *user) {
    userPool.insert(pair<UserId, UserInfo*>(user->id, user));	
}

string toString(UserId userId) {
    return userId;
}

string toString(ChatId chatId) {
    return std::to_string(chatId);
}

bool equalId(UserId id0, UserId id1) {
    return (id0.compare(id1) == 0);
}

