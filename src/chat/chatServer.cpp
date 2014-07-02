#include "chatServer.h"

using boost::thread;
using namespace chat;
using socketlibrary::LuxSocket;

using std::cout;
using std::endl;

#define DEBUG

// Stores all online user information here
map<UserId, UserInfo *> userPool;

// Stores all chats in the list
list<SubServer *> subServList;

// Socket
LuxSocket *mainSock;

// Creates chat for users in the list
// Sends back the chat data so that all clients and create coressponding chat box
Chat* createChat(const UserInfo &user, vector<UserId> &idList, 
		 MESSAGE_TYPE &msgType);

// Find a sub server for chat 
SubServer* findSubServer();

// Create a new sub server
SubServer* createNewSubServer();

// Make chat info into a string
void makeChatInfo(ChatId chatId, string &msgChatInfo);

void sendToAll(BYTE *buf, size_t len, LuxSocket *sock, Chat &chat);
// Sends message to all users expect the sender in this chat
void sendToOthers(BYTE *buf, size_t len, LuxSocket *sock, Chat &chat, 
		  UserId &senderId);

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

void subServerHandleClientRequest(BYTE *buf, size_t len, 
				  sockaddr_in *tmpAddr, SubServer *serv);

void addUserToChat(Chat &chat, vector<UserId> idList, MESSAGE_TYPE &msgType);

void quitChat(UserInfo &user, Chat &chat, MESSAGE_TYPE &msgType);


// Helper functions

UserInfo* findUser(UserId id);

bool verifyUser(UserInfo *userPtr, sockaddr_in &cliAddr);
int sockAddrCmp(const sockaddr_in &a, const sockaddr_in &b);

// Returns a new chat id
ChatId getNewChatId();
string toString(UserId userId);
string toString(ChatId chatId);
bool equalId(UserId id0, UserId id1);
ThreadMgr *threads;

void sigint_handler(int sig) {
    cout << "exiting..." << endl;
    if (threads) {
	delete threads;
    }
    exit(0);
}
int main() {
    unsigned short port = 3000;
    mainSock = new LuxSocket(port);
    struct sockaddr_in cliAddr;
    char *buf = new char[BUFSIZE];
    string serverInfo(mainSock->getAddress());

    signal(SIGINT, sigint_handler);

    threads = new ThreadMgr();
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
    
    ChatPacket packet(buf, len);

    packet.parseMessage(msgId, senderId, reqType, msgType);
        
    UserInfo *user = findUser(senderId);
    if (reqType != CONNECT) {
	if (!verifyUser(user, cliAddr)) {
	    // User ip changed or isOnline state changed,
	    // requires user to reconnect
#ifdef DEBUG
	    cout << "User request not valid\n";
#endif
	    msgType = RE_CONNECT;
	    packet.makeMessage(msgId, senderId, reqType,
			       msgType, "Not connected.");
	    mainSock->send(packet.getData(), packet.getLen(), &cliAddr);
	    delete[] buf;
	    return;
	}
    }

    // Main thread only handles request other chat
    switch (reqType) {	    
	case CONNECT: {
	    unsigned short recvPort, pollPort;
	    packet.parsePortNum(recvPort, pollPort);
#ifdef DEBUG
	    cout << "User " << senderId << " connecting.";
	    cout << "Ports: " << recvPort << " " << pollPort << "\n";
#endif	    
	    if (connect(senderId, cliAddr, recvPort, pollPort)) {
		// Successfully connected
		msgType = CONFIRM;
		packet.makeMessage(msgId, senderId, reqType, 
				   msgType, "Welcome.");
#ifdef DEBUG
		cout << "Connected.\n";
#endif

	    }		
	    else {
		msgType = CONNECT_ERROR;
		packet.makeMessage(msgId, senderId, reqType,
				   msgType, "Connection error.");
#ifdef DEBUG
		cout << "Error while connecting.\n";
#endif

	    }
	    mainSock->send(packet.getData(), packet.getLen(), &cliAddr);
	    break;
	}
	case DISCONNECT: {
#ifdef DEBUG
	    cout << "User " << senderId << "disconnecting...\n"; 
#endif
	    disconnect(*user);
	    msgType = CONFIRM;
	    packet.makeMessage(msgId, senderId, reqType, 
			       msgType, "Disconnect");
	    mainSock->send(packet.getData(), packet.getLen(), &(user->addr));
	    break;
	}
	case POLLING: {
	    unsigned short recvPort, pollPort;
	    packet.parsePortNum(recvPort, pollPort);
	    user->isOnline = true;
	    updateUserPorts(*user, recvPort, pollPort);
	    msgType = CONFIRM;
	    packet.makeMessage(msgId, senderId, reqType, msgType, "OK");
	    mainSock->send(packet.getData(), packet.getLen(), 
			   &(user->pollAddr));
	    break;
	}
	case CREATE_CHAT: {
	    vector<UserId> idList;

	    packet.parseUserList(idList);
#ifdef DEBUG
	    cout << "User " << senderId << " creating a chat...\n"; 
	    cout << "User List:\n";
	    for (vector<UserId>::iterator it = idList.begin();
		 it != idList.end();
		 it ++) {
		cout << (*it) << endl;
	    }
#endif	    
	    Chat *chat = createChat(*user, idList, msgType);  
	    packet.makeMessage(msgId, senderId, reqType, msgType);
	    if (chat != NULL) {
	        packet.appendMessage(chat->toBytes());
		for (vector<UserId>::iterator it = idList.begin();
		     it != idList.end();
		     it ++) {
		    UserInfo *user = findUser(*it);
		    if (user != NULL) {
			cout << "User Id: " << user->id << endl;
			cout << "User port: " << user->addr.sin_port << endl;
			mainSock->send(packet.getData(), packet.getLen(), 
				       &(user->addr));
		    }
		}	
	    }
	    else {
	        mainSock->send(packet.getData(), packet.getLen(), &(user->addr));
	    }
	    break;
	}
	default: {
	    break;
	}
    }
    delete[] buf;
}

void startSubServerThread(SubServer *serv) {
    LuxSocket *sock = serv->getSocket();
    struct sockaddr_in cliAddr;	
    BYTE *buf = new BYTE[BUFSIZE];
    cout << "SubServer " << serv->getId() << "working." << endl;
    cout << "Port " << sock->getPortNum() << endl;
    while (1) {	    
	int n = sock->receive(buf, BUFSIZE, &cliAddr);
	BYTE *tmpBuf = new BYTE[n];
	memcpy(tmpBuf, buf, n);
	sockaddr_in *tmpAddr = new sockaddr_in(cliAddr);
	thread(subServerHandleClientRequest, tmpBuf, n, tmpAddr, serv).detach();
    }    
}

void subServerHandleClientRequest(BYTE *buf, size_t len, sockaddr_in *tmpAddr, 
				  SubServer *serv) {
    sockaddr_in cliAddr(*tmpAddr);
    delete tmpAddr;
    // Parse message and handle request
    LuxSocket *sock = serv->getSocket();
    MsgId msgId;
    UserId senderId;
    REQUEST_TYPE reqType;
    MESSAGE_TYPE msgType;
    
    ChatPacket packet(buf, len);

    packet.parseMessage(msgId, senderId, reqType, msgType);

    UserInfo *user = findUser(senderId);
    if (!verifyUser(user, cliAddr)) {
	// User ip changed, require user to reconnect
	msgType = RE_CONNECT;
	packet.makeMessage(msgId, senderId, reqType,
				msgType, "Not connected");
	sock->send(packet.getData(), packet.getLen(), &cliAddr);
	delete[] buf;
	return;
    }
	
    ChatId chatId;
    packet.parseChatId(chatId);
    Chat *chat = serv->getChat(chatId);

    if (chat == NULL) {
	msgType = CHAT_NOT_EXIST;
	packet.makeMessage(msgId, senderId, reqType,
			   msgType, "Chat does not exist.");
	sock->send(packet.getData(), packet.getLen(), &(user->addr));
	delete[] buf;
	return;
    }
    switch (reqType) {
	case QUIT_CHAT: {	    
	    quitChat(*user, *chat, msgType);
	    packet.makeMessage(msgId, senderId, reqType, msgType);
	    if (msgType == CONFIRM) {
		sendToAll(packet.getData(), packet.getLen(), sock, *chat);
	    }
	    else {
		sock->send(packet.getData(), packet.getLen(), &(user->addr));
	    }
	    break;
	}
	case ADD_USER_TO_CHAT: {
	    // TODO: send different confirm message to new added users
	    vector<UserId> idList;
	    packet.parseAddUserList(idList);
	    addUserToChat(*chat, idList, msgType);
	    packet.makeMessage(msgId, senderId, reqType, msgType);
	    packet.appendMessage(chat->toBytes());
	    if (msgType == CONFIRM) {
		sendToAll(packet.getData(), packet.getLen(), sock, *chat);
	    }
	    else {
		sock->send(packet.getData(), packet.getLen(), &(user->addr));
	    }
	    break;
	}
	case SEND_MESSAGE: {
	    sendToOthers(packet.getData(), packet.getLen(), sock, *chat, 
			 senderId);
	    msgType = CONFIRM;
	    packet.makeMessage(msgId, senderId, reqType, msgType, "");
	    sock->send(packet.getData(), packet.getLen(), &(user->addr));
	    break;
	}
	default: {
	    break;
	}
    }
    delete[] buf;
}

Chat* createChat(const UserInfo &user, vector<UserId> &idList, 
		MESSAGE_TYPE &msgType) {
    SubServer* serv = findSubServer();
    if (serv == NULL) {
	// No server is available
	msgType = NO_MORE_SERVERS;
	return NULL;
    }

    Chat *chat = new Chat();
    if (chat->emptySpace() < idList.size()) {
	msgType = EXCEED_CHAT_CAP;
	delete chat;
	return NULL;
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
    msgType = CHAT_INFO;
    return chat;
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
    thread *t = new thread(startSubServerThread, server);
    threads->push_back(t);
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
    user->isOnline = true;
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
void addUserToChat(Chat &chat, vector<UserId> idList, MESSAGE_TYPE &msgType) {
    if (chat.emptySpace() < idList.size()) {
	msgType = EXCEED_CHAT_CAP;
	return;
    }
    
    for (vector<UserId>::iterator it = idList.begin();
	 it != idList.end();
	 it ++) {
	chat.insertUser(*it);
    }
    
    msgType = CONFIRM;
}

void quitChat(UserInfo &user, Chat &chat, MESSAGE_TYPE &msgType) {
    for (vector<UserId>::iterator it = chat.getList().begin();
	 it != chat.getList().end();
	 it ++) {
	if (equalId(user.id, (*it))) {
	    chat.eraseUser(it);
	    msgType = CONFIRM;
	    return;
	}
    }
    msgType = USER_NOT_IN_CHAT;
}
// Functions of sub server thread end


// Shared funtcions
void sendToOthers(BYTE *buf, size_t len, LuxSocket *sock, Chat &chat, 
		  UserId &senderId) {
    for (vector<UserId>::iterator it = chat.getList().begin();
	 it != chat.getList().end();
	 it ++) {
	UserInfo *user = findUser(*it);       
	if (user != NULL) {
	    if (!equalId(senderId, user->id)) {
		cout << "Send to: " << user->id << endl;
		sock->send(buf, len, &(user->addr));
	    }
	}
    }    
}

void sendToAll(BYTE *buf, size_t len, LuxSocket *sock, Chat &chat) {
    for (vector<UserId>::iterator it = chat.getList().begin();
	 it != chat.getList().end();
	 it ++) {
	UserInfo *user = findUser(*it);
	if (user != NULL) {
	    sock->send(buf, len, &(user->addr));
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

bool verifyUser(UserInfo *userPtr, sockaddr_in &cliAddr) {
    if (userPtr == NULL) {
	return false;
    }
    if (!userPtr->isOnline) {
	return false;
    }
    if (sockAddrCmp(userPtr->addr, cliAddr) == 0) {
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
