#include "chatServer.h"

using namespace chat;
using socketlibrary::LuxSocket;

enum REQUEST_TYPE{
    CONNECT,
    DISCONNECT,    
    KEEP,
    CREATE_CHAT,
    QUIT_CHAT,
    ADD_USER_TO_CHAT,
    SEND_MESSAGE
};

enum MESSAGE_TYPE {    
    NO_MORE_SERVERS,
    EXCEED_CHAT_CAP,
    CHAT_NOT_EXIST,
    USER_NOT_IN_CHAT,
    CONNECT_ERROR,
    USER_LIST,
    TEXT_MSG,
    AUDIO_MSG,
    VIDEO_MSG,
    CHAT_INFO,
    SERVER_INFO,
    RE_CONNECT,
    CONFIRM
};

const int EUID_LEN = 10; // EUID length, not determined yet

// Stores all online user information here
map<UserId, UserInfo *> userPool;

// Stores all chats in the list
list<SubServer *> subServList;

// Socket
LuxSocket *mainSock;

// Parse meesage from client and handle request
// -----------------------------------------------------------------------------
// Packet format:
//    4 byte     xx* byte      1 byte        1 byte          message/parameters 
//  Message ID  sender EUID  request type  message type    ....................
// -----------------------------------------------------------------------------
// Message/Parameters format:
// USER_LIST :     4 byte         xx* byte xx* byte 
//               user number    EUID0   EUID1    ....
// QUIT_CHAT:  4 byte
//             chatId
// TEXT_MSG,   
// AUDIO_MSG,
// VIDEO_MSG : 4 byte         conetnt
//             chatId     ...................
// -----------------------------------------------------------------------------
// xx: length of EUID (not determined yet)
//
void parseMessage(char *buf, MsgId &msgId, UserId &senderId, 
		  BYTE &reqType, BYTE &msgType, char *message);
void parseChatId(char *message, ChatId &id);
void parseUserList(char *message, vector<UserId> &idList);
void makeMessage(char *buf, MsgId &msgId, UserId &senderId, 
		  BYTE &reqType, BYTE &msgType, const char *message);

// Creates chat for users in the list
// Sends back the chat data so that all clients and create coressponding chat box
void createChat(const UserInfo &user, vector<UserId> &idList,
		BYTE &msgType, char *message);

// Find a sub server for chat 
SubServer* findSubServer();

// Create a new sub server
SubServer* createNewSubServer();

// Make chat info into a string
void makeChatInfo(ChatId chatId, string &msgChatInfo);

// Send message to a client
// -----------------------------------------------------------------------------
// Packet format:
//    4 byte     xx* byte      1 byte        1 byte          message/parameters 
//  Message ID  sender EUID  request type  message type    ....................
// -----------------------------------------------------------------------------
// Message/Parameters format:
// CHAT_INFO :  4 bype   4 byte    4 byte      xx* byte   xx* byte
//              chatID  capacity  user number   EUID0      EUID1   ....
// SERVER_INFO: 4 byte   2 byte
//               ipv4     port
// TEXT_MSG,
// AUDIO_MSG,
// VIDEO_MSG : ...................
// -----------------------------------------------------------------------------
// xx: length of EUID (not determined yet)

void sendToAll(char *buf, LuxSocket &sock, Chat &chat);
// Sends message to all users expect the sender in this chat
void sendToOthers(char *buf, LuxSocket &sock, Chat &chat, UserId &senderId);

// Update chats in chat list
// Check if users are still online, if not remove from list
// Check if chat is still valid
void updateChatPool();

// Update users in user pool
// Gets user information from social network server and updates users status
void updateUserPool();
    
// Add new user into user pool
bool connect(UserId &id, sockaddr_in &addr);

// Clear user online status
bool disconnect(UserInfo &user);

// Sub server thread
void *startSubServerThread(void *vargp);

void addUserToChat(Chat &chat, vector<UserId> idList, BYTE &msgType, 
		   char *msg);
void quitChat(UserInfo &user, Chat &chat, BYTE &msgType, char *msg);


// Helper functions

UserInfo* findUser(UserId id);

bool verifyUser(UserInfo &user, sockaddr_in &cliAddr);
int sockAddrCmp(const sockaddr_in &a, const sockaddr_in &b);


// Returns a new chat id
ChatId getNewChatId();
string toString(UserId userId);
string toString(ChatId chatId);
bool equalId(UserId id0, UserId id1);

int main() {
    unsigned short port = 8000;
    mainSock = new LuxSocket(port);
    struct sockaddr_in cliAddr;
    char *buf = new char[BUFSIZE];
    string serverInfo(mainSock->getAddress());    
    for (int i = 0; i < sizeof(port); i++) {	
	serverInfo.push_back(*((char*)port + i));
    }

    while (1) {
	// This is the main server.
	// Receive data from clients and do whatever need to do.
	mainSock->receive(buf, BUFSIZE, &cliAddr);
	
	// Parse message and handle request
	MsgId msgId;
	UserId senderId;
	char *message;	
	BYTE reqType, msgType;
	parseMessage(buf, msgId, senderId, reqType, msgType, message);

	UserInfo *user = findUser(senderId);
	if (reqType != CONNECT) {
	    if (!verifyUser(*user, cliAddr)) {
		// User ip changed, require user to reconnect
		msgType = RE_CONNECT;
		makeMessage(buf, msgId, senderId, reqType,
			    msgType, "");
		mainSock->send(buf, &cliAddr);
		continue;
	    }
	}
	
	// Main thread only handles request other chat
	switch (reqType) {	    
	    case CONNECT: {
		if (connect(senderId, cliAddr)) {
		    // Successfully connected
		    msgType = SERVER_INFO;
		    makeMessage(buf, msgId, senderId, reqType, 
				msgType, serverInfo.c_str());
		}		
		else {
		    msgType = CONNECT_ERROR;
		    makeMessage(buf, msgId, senderId, reqType,
				msgType, "Connection error.");
		}
		mainSock->send(buf, &cliAddr);
		break;
	    }
	    case DISCONNECT: {
		break;
	    }
	    case KEEP: {
		break;
	    }
	    case CREATE_CHAT: {
		vector<UserId> idList;
		parseUserList(message, idList);
		createChat(*user, idList, msgType, message);
		makeMessage(buf, msgId, senderId, reqType,
			    msgType, message);		
		for (vector<UserId>::iterator it = idList.begin();
		     it != idList.end();
		     it ++) {
		    UserInfo *user = findUser(*it);
		    if (user != NULL) {
			mainSock->send(buf, &(user->addr));
		    }
		}
		break;
	    }
	    default: {
		break;
	    }
	}
    }
    return 0;
}

void *startSubServerThread(void *vargp) {
    SubServer *serv = (SubServer*)vargp;
    LuxSocket sock = serv->getSocket();	
    struct sockaddr_in cliAddr;	
    char *buf = new char[BUFSIZE];
	
    while (1) {	    
	sock.receive(buf, BUFSIZE, &cliAddr);

	// Parse message and handle request
	MsgId msgId;
	UserId senderId;
	char *message;
	BYTE reqType, msgType;
	parseMessage(buf, msgId, senderId, reqType, msgType, message);

	UserInfo *user = findUser(senderId);
	if (!verifyUser(*user, cliAddr)) {
	    // User ip changed, require user to reconnect
	    msgType = RE_CONNECT;
	    makeMessage(buf, msgId, senderId, reqType,
			msgType, "");
	    sock.send(buf, &cliAddr);
	    continue;
	}
	
	ChatId chatId;
	parseChatId(message, chatId);
	Chat *chat = serv->getChat(chatId);

	if (chat == NULL) {
	    msgType = CHAT_NOT_EXIST;
	    *message = '\0';
	    makeMessage(buf, msgId, senderId, reqType,
			msgType, message);
	    sock.send(buf, &cliAddr);
	    continue;
	}
	switch (reqType) {
	    case QUIT_CHAT: {
		quitChat(*user, *chat, msgType, message);
		makeMessage(buf, msgId, senderId, reqType, msgType, message);
		if (msgType == CONFIRM) {
		    sendToAll(buf, sock, *chat);
		}
		else {
		    sock.send(buf, &cliAddr);
		}
		break;
	    }
	    case ADD_USER_TO_CHAT: {
		vector<UserId> idList;
		parseUserList(message + sizeof(chatId), idList);
		addUserToChat(*chat, idList, msgType, message);
		makeMessage(buf, msgId, senderId, reqType, msgType, message);	
		if (msgType == CONFIRM) {
		    sendToAll(buf, sock, *chat);
		}
		else {
		    sock.send(buf, &cliAddr);
		}
		break;
	    }
	    case SEND_MESSAGE: {
		sendToOthers(buf, sock, *chat, senderId);
		msgType = CONFIRM;
		makeMessage(buf, msgId, senderId, reqType, msgType, "");
		sock.send(buf, &cliAddr);
		break;
	    }
	    default: {
		break;
	    }
	}
    }

    return NULL;
}

// Function sof main thread 
void parseMessage(char *buf, MsgId &msgId, UserId &senderId, 
		  BYTE &reqType, BYTE &msgType, char *message) {
    // No error handling for now
    int p = 0;
    for (int i = 0; i < sizeof(MsgId); i++) {
	*((char *)msgId + i) = buf[p++];
    }
    for (int i = 0; i < EUID_LEN; i++) {
	senderId.push_back(buf[p++]);
    }
    reqType = buf[p++];
    msgType = buf[p++];
    message = buf + p;
}

void parseChatId(char *message, ChatId &id) {
    for (int i = 0; i < sizeof(id); i++) {
	*((char *)id + i) = message[i];
    }
}

void parseUserList(char *message, vector<UserId> &idList) {
    // No error handling for now
    int count;
    int p = 0;
    for (int i = 0; i < sizeof(count); i++) {
	*((char*)count + i) = message[p++];
    }
    for (int i = 0; i < count; i++) {
	UserId tmp;
	for (int j = 0; j < EUID_LEN; j++) {
	    tmp.push_back(message[p++]);
	}
	idList.push_back(tmp);
    }
}

void makeMessage(char *buf, MsgId &msgId, UserId &senderId, 
		 BYTE &reqType, BYTE &msgType, const char *message) {
    //memset(buf, 0, strlen(buf));
    int p = 0;
    for (int i = 0; i < sizeof(MsgId); i++) {
	buf[p++] = *((char *)msgId + i);
    }
    for (int i = 0; i < EUID_LEN; i++) {
	buf[p++] = senderId[i];
    }
    buf[p++] = reqType;
    buf[p++] = msgType;
    if (buf + p != message) {
	strcpy(buf + p, message);
    }
}


void createChat(const UserInfo &user, vector<UserId> &idList,
		BYTE &msgType, char *message) {
    SubServer* serv = findSubServer();
    if (serv == NULL) {
	// No server is available
	msgType = NO_MORE_SERVERS;
	*message = '\0';
	return;
    }
    
    Chat *chat = new Chat();
    if (chat->emptySpace() < idList.size()) {
	msgType = EXCEED_CHAT_CAP;
	*message = '\0';
	delete chat;
	return;
    }
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
    strcpy(message, chat->toString().c_str());
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
	    user->isOnline = (*it).isOnline;
	    user->addr = (*it).addr;	 
	}
	else {
	    // New user
	    UserInfo *user = new UserInfo();
	    user->id = (*it).id;
	    user->isOnline = (*it).isOnline;
	    user->addr = (*it).addr;
	    userPool.insert(pair<UserId, UserInfo*>(user->id,user));
	}
    }    
}
    
bool connect(UserId &id, sockaddr_in &addr) {
    UserInfo *user = new UserInfo();
    user->id = id;
    user->addr = addr;    
    userPool.insert(pair<UserId, UserInfo*>(user->id, user));
    return true;
}
// Functions of main thread end

// Funtcions of sub server thread
void addUserToChat(Chat &chat, vector<UserId> idList, BYTE &msgType, 
		   char *msg) {
    if (chat.emptySpace() < idList.size()) {
	msgType = EXCEED_CHAT_CAP;
	*msg = '\0';
	return;
    }
    
    for (vector<UserId>::iterator it = idList.begin();
	 it != idList.end();
	 it ++) {
	chat.insertUser(*it);
    }
    
    msgType = CONFIRM;
    strcpy(msg, chat.toString().c_str());
}

void quitChat(UserInfo &user, Chat &chat, BYTE &msgType, char *msg) {
    for (vector<UserId>::iterator it = chat.getList().begin();
	 it != chat.getList().end();
	 it ++) {
	if (equalId(user.id, (*it))) {
	    chat.eraseUser(it);
	    msgType = CONFIRM;
	    *msg = '\0';
	    return;
	}
    }
    msgType = USER_NOT_IN_CHAT;
    *msg = '\0';
}
// Functions of sub server thread end


// Shared funtcions
void sendToOthers(char *buf, LuxSocket &sock, Chat &chat, UserId &senderId) {
    for (vector<UserId>::iterator it = chat.getList().begin();
	 it != chat.getList().end();
	 it ++) {
	UserInfo *user = findUser(*it);       
	if (user != NULL) {
	    if (!equalId(senderId, user->id)) {
		sock.send(buf, &(user->addr));
	    }
	}
    }    
}

void sendToAll(char *buf, LuxSocket &sock, Chat &chat) {
    for (vector<UserId>::iterator it = chat.getList().begin();
	 it != chat.getList().end();
	 it ++) {
	UserInfo *user = findUser(*it);
	if (user != NULL) {
	    sock.send(buf, &(user->addr));
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

UserInfo* findUser(UserId id) {
    map<UserId, UserInfo*>::iterator it = userPool.find(id);
    if (it == userPool.end()) {
	return NULL;
    }
    return it->second;
}

bool verifyUser(UserInfo &user, sockaddr_in &cliAddr) {
    if (sockAddrCmp(user.addr, cliAddr) == 0) {
	if (user.addr.sin_port != cliAddr.sin_port) {
	    user.addr.sin_port = cliAddr.sin_port;
	}
	return true;
    }
    return false;
}

int sockAddrCmp(const sockaddr_in &a, const sockaddr_in &b) {
    if (a.sin_family != b.sin_family) {
	return a.sin_family - b.sin_family;
    }
    if (a.sin_family == AF_INET) {
	// IPV4
	return a.sin_addr.s_addr - b.sin_addr.s_addr;
    }
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
