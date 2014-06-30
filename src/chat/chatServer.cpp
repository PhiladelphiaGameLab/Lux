#include "chatServer.h"
#include <boost/thread/thread.hpp>

using boost::thread;
using namespace chat;
using socketlibrary::LuxSocket;

using std::cout;
using std::endl;

// Stores all online user information here
map<UserId, UserInfo *> userPool;

// Stores all chats in the list
list<SubServer *> subServList;

// Socket
LuxSocket *mainSock;

// Creates chat for users in the list
// Sends back the chat data so that all clients and create coressponding chat box
void createChat(const UserInfo &user, vector<UserId> &idList,
		MESSAGE_TYPE &msgType, char *message);

// Find a sub server for chat 
SubServer* findSubServer();

// Create a new sub server
SubServer* createNewSubServer();

// Make chat info into a string
void makeChatInfo(ChatId chatId, string &msgChatInfo);

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
bool connect(UserId &id, sockaddr_in &addr, unsigned short port, 
	     unsigned short pollPort);

// Clear user online status
bool disconnect(UserInfo &user);

void updateUserPorts(UserInfo &user, unsigned short recvPort, 
		     unsigned short pollPort);


void handleClientRequest(BYTE *buf, size_t len, sockaddr_in *tmpAddr);
// Sub server thread
void startSubServerThread(SubServer *serv);

void subServerHandleClientRequest(char *buf, sockaddr_in *tmpAddr, 
				  SubServer *serv);

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
    unsigned short port = 3000;
    mainSock = new LuxSocket(port);
    struct sockaddr_in cliAddr;
    char *buf = new char[BUFSIZE];
    string serverInfo(mainSock->getAddress());    

    while (1) {
	// This is the main server.
	// Receive data from clients and do whatever need to do.
	size_t n = mainSock->receive(buf, BUFSIZE, &cliAddr);
	
	BYTE *tmpBuf = new BYTE[n];
	memcpy(tmpBuf, buf, n);
	sockaddr_in *tmpAddr = new sockaddr_in(cliAddr);
	thread(handleClientRequest, tmpBuf, n, tmpAddr).detach();
    }
    return 0;
}

void handleClientRequest(BYTE *buf, size_t len, sockaddr_in* tmpAddr) {
    // Parse message and handle request
    sockaddr_in cliAddr(*tmpAddr);
    delete tmpAddr;
    string serverInfo(mainSock->getAddress());
    MsgId msgId;
    UserId senderId;
    REQUEST_TYPE reqType;
    MESSAGE_TYPE msgType;

    ChatPacket::parseMessage(buf, len, msgId, senderId, reqType, msgType);
    
    cout << len << endl;
    cout << msgId << endl;
    cout << senderId << endl;
    cout << reqType << endl;
    cout << msgType << endl;
    
    UserInfo *user = findUser(senderId);
    if (reqType != CONNECT) {
	if (!verifyUser(*user, cliAddr)) {
	    // User ip changed or isOnline state changed,
	    // requires user to reconnect
	    msgType = RE_CONNECT;
	    len = ChatPacket::makeMessage(buf, msgId, senderId, reqType,
					  msgType, "");
	    mainSock->send(buf, len, &cliAddr);
	    delete[] buf;
	    return;
	}
    }

    // Main thread only handles request other chat
    switch (reqType) {	    
	case CONNECT: {
	    unsigned short recvPort, pollPort;
	    ChatPacket::parsePortNum(buf, recvPort, pollPort);
	    
	    if (connect(senderId, cliAddr, recvPort, pollPort)) {
		// Successfully connected
		msgType = SERVER_INFO;
		len = ChatPacket::makeMessage(buf, msgId, senderId, reqType, 
					      msgType, serverInfo.c_str());
	    }		
	    else {
		msgType = CONNECT_ERROR;
		len = ChatPacket::makeMessage(buf, msgId, senderId, reqType,
					      msgType, "Connection error.");
	    }	 
	    mainSock->send(buf, len, &cliAddr);
	    break;
	}
	case DISCONNECT: {
	    disconnect(*user);
	    msgType = SERVER_INFO;
	    len = ChatPacket::makeMessage(buf, msgId, senderId, reqType, 
			msgType, serverInfo.c_str());
	    mainSock->send(buf, len, &(user->addr));
	    break;
	}
	case POLLING: {
	    unsigned short recvPort, pollPort;
	    ChatPacket::parsePortNum(buf, recvPort, pollPort);
	    user->isOnline = true;
	    updateUserPorts(*user, recvPort, pollPort);
	    msgType = CONFIRM;
	    len = ChatPacket::makeMessage(buf, msgId, senderId, reqType, msgType, "OK");
	    mainSock->send(buf, len, &(user->pollAddr));
	    break;
	}
	case CREATE_CHAT: {
	    vector<UserId> idList;
	    char *text = new char[BUFSIZE];
	    ChatPacket::parseUserList(buf, idList);
	    createChat(*user, idList, msgType, text);
	    len = ChatPacket::makeMessage(buf, msgId, senderId, reqType,
			msgType, text);		
	    for (vector<UserId>::iterator it = idList.begin();
		 it != idList.end();
		 it ++) {
		UserInfo *user = findUser(*it);
		if (user != NULL) {
		    mainSock->send(buf, len, &(user->addr));
		}
	    }
	    delete[] text;
	    break;
	}
	default: {
	    break;
	}
    }
    delete[] buf;
}

void startSubServerThread(SubServer *serv) {
    LuxSocket sock = serv->getSocket();	
    struct sockaddr_in cliAddr;	
    char *buf = new char[BUFSIZE];
	
    while (1) {	    
	int n = sock.receive(buf, BUFSIZE, &cliAddr);
	char *tmpBuf = new char[strlen(buf) + 1];
	memcpy(tmpBuf, buf, n);
	sockaddr_in *tmpAddr = new sockaddr_in(cliAddr);
	thread(subServerHandleClientRequest, tmpBuf, tmpAddr, serv).detach();
    }    
}

void subServerHandleClientRequest(char *buf, sockaddr_in *tmpAddr, 
				  SubServer *serv) {
    sockaddr_in cliAddr(*tmpAddr);
    delete tmpAddr;
    // Parse message and handle request
    LuxSocket sock = serv->getSocket();
    MsgId msgId;
    UserId senderId;
    char *message;
    BYTE reqType, msgType;
    /*
    ChatPacket::parseMessage(buf, msgId, senderId, reqType, msgType, message);

    UserInfo *user = findUser(senderId);
    if (!verifyUser(*user, cliAddr)) {
	// User ip changed, require user to reconnect
	msgType = RE_CONNECT;
	ChatPacket::makeMessage(buf, msgId, senderId, reqType,
		    msgType, "");
	sock.send(buf, &cliAddr);
	delete[] buf;
	return;
    }
	
    ChatId chatId;
    ChatPacket::parseChatId(message, chatId);
    Chat *chat = serv->getChat(chatId);

    if (chat == NULL) {
	msgType = CHAT_NOT_EXIST;
	*message = '\0';
	ChatPacket::makeMessage(buf, msgId, senderId, reqType,
		    msgType, message);
	sock.send(buf, &cliAddr);
	delete[] buf;
	return;
    }
    switch (reqType) {
	case QUIT_CHAT: {
	    quitChat(*user, *chat, msgType, message);
	    ChatPacket::makeMessage(buf, msgId, senderId, reqType, msgType, message);
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
	    ChatPacket::parseUserList(message + sizeof(chatId), idList);
	    addUserToChat(*chat, idList, msgType, message);
	    ChatPacket::makeMessage(buf, msgId, senderId, reqType, msgType, message);	
	    if (msgType == CONFIRM) {
		sendToAll(buf, sock, *chat);
	    }
	    else {
		sock.send(buf, &cliAddr);
	    }
	    break;
	}
	case SEND_MESSAGE:
	{
	    sendToOthers(buf, sock, *chat, senderId);
	    msgType = CONFIRM;
	    ChatPacket::makeMessage(buf, msgId, senderId, reqType, msgType, "");
	    sock.send(buf, &cliAddr);
	    break;
	}
	default: {
	    break;
	}
    }
    delete[] buf;
    */
}

// Functions of main thread 
void parseMessage(char *buf, MsgId &msgId, UserId &senderId, 
		  BYTE &reqType, BYTE &msgType, char *message) {
    // No error handling for now
    int p = 0;
    for (int i = 0; i < sizeof(MsgId); i++) {
	*((char*)&msgId + i) = buf[p++];
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
	*((char*)&id + i) = message[i];
    }
}

void parseUserList(char *message, vector<UserId> &idList) {
    // No error handling for now
    int count;
    int p = 0;
    for (int i = 0; i < sizeof(count); i++) {
	*((char*)&count + i) = message[p++];
    }
    for (int i = 0; i < count; i++) {
	UserId tmp;
	for (int j = 0; j < EUID_LEN; j++) {
	    tmp.push_back(message[p++]);
	}
	idList.push_back(tmp);
    }
}

void parsePortNum(char *message, unsigned short &cliPort, 
		  unsigned short &pollPort) {
    int p = 0;
    for (int i = 0; i < sizeof(cliPort); i++) {
	*((char*)&cliPort + i) = message[p++];
    }
    for (int i = 0; i < sizeof(pollPort); i++) {
	*((char*)&pollPort + i) = message[p++];
    }
}

void makeMessage(char *buf, MsgId &msgId, UserId &senderId, 
		 BYTE &reqType, BYTE &msgType, const char *message) {
    //memset(buf, 0, strlen(buf));
    int p = 0;
    for (int i = 0; i < sizeof(MsgId); i++) {
	buf[p++] = *((char*)&msgId + i);
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
		MESSAGE_TYPE &msgType, char *message) {
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
    // Start sub server thread
    thread(startSubServerThread, server).detach();
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
    
bool connect(UserId &id, sockaddr_in &addr, unsigned short port, 
	     unsigned short pollPort) {
    UserInfo *user = findUser(id);
    if (user == NULL) {
	user = new UserInfo();
    }
    user->id = id;
    user->addr = addr;
    user->addr.sin_port = htons(port);
    user->pollAddr = addr;
    user->pollAddr.sin_port = htons(pollPort);
    userPool.insert(pair<UserId, UserInfo*>(user->id, user));
    addr.sin_port = user->addr.sin_port;
    return true;
}

bool disconnect(UserInfo &user) {
    user.isOnline = false;
    return true;
}

void updateUserPorts(UserInfo &user, unsigned short recvPort, 
		     unsigned short pollPort) {
    recvPort = htons(recvPort);
    pollPort = htons(pollPort);
    if (user.addr.sin_port != recvPort) {
	user.addr.sin_port = recvPort;
    }
    if (user.pollAddr.sin_port != pollPort) {
	user.pollAddr.sin_port = pollPort;
    }    
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
    if (!user.isOnline) {
	return false;
    }
    if (sockAddrCmp(user.addr, cliAddr) == 0) {
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
    return -1;
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
