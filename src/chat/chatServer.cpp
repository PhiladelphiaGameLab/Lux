#include "chatServer.h"
#include <arpa/inet.h>  // for inet_ntoa

using boost::thread;
using namespace chat;
using socketlibrary::LuxSocket;

using std::cout;
using std::endl;

#define DEBUG

void sigint_handler(int sig) {
    cout << "exiting..." << endl;
    exit(0);
}

void ChatServer::run() {
    struct sockaddr_in cliAddr;
    BYTE *buf = new BYTE[BUFSIZE];
    
    // Start update function
    thread(&ChatServer::updateAll, this).detach();

#ifdef DEBUG
	// create a public char room
	do {
	    std::lock_guard<std::mutex> lock(_subServerMutex);    
	    SubServer* serv = findSubServer();
	    if (serv == nullptr) {
		// No server is available
		cout << "Error: Creating test server.\n";
		exit(1);
	    }

	    Chat *chat = new Chat();
	    
	    chat->setAddress(serv->getAddress());
	    chat->setPortNum(serv->getPortNum());

	    vector<UserId> arr{"__"};
	    chat->insertUser(arr);
            
	    // Insert new chat into sub server
	    serv->insertChat(*chat);
	} while(0);
#endif

    
    while (1) {
	// Receive data from clients and do whatever need to do.	
	size_t n = _mainSock->receive(buf, BUFSIZE, &cliAddr);

	cout << "IP: " << inet_ntoa(cliAddr.sin_addr) << ":" << ntohs(cliAddr.sin_port) << "\n";	
	BYTE *tmpBuf = new BYTE[n];
	memcpy(tmpBuf, buf, n);
	sockaddr_in *tmpAddr = new sockaddr_in(cliAddr);
	thread(&ChatServer::mainRequestHandler, this,
	       tmpBuf, n, tmpAddr).detach();
    }
}

UserInfo* ChatServer::connect(UserId &id, sockaddr_in &addr, 
			      unsigned short port, 
			      unsigned short pollPort) {
    UserInfo *user = findUser(id);
    if (user == nullptr) {
	user = new UserInfo();
    }
    user->id = id;
    user->isOnline = true;
    user->addr = addr;
    user->addr.sin_port = htons(port);
    user->pollAddr = addr;
    user->pollAddr.sin_port = htons(pollPort);       
    _userPool.insert(pair<UserId, UserInfo*>(user->id, user));
    return user;
}

bool ChatServer::disconnect(UserInfo &user) {
    user.isOnline = false;
    return true;
}

Chat* ChatServer::createChat(const UserInfo &user, 
			     const vector<UserId> &idArray, 
			     MESSAGE_TYPE &msgType) {
    // Exclusiva access to sub servers
    std::lock_guard<std::mutex> lock(_subServerMutex);
    
    SubServer* serv = findSubServer();
    
    if (serv == nullptr) {
	// No server is available
	msgType = NO_MORE_SERVERS;
	return nullptr;
    }

    Chat *chat = new Chat();
    if (chat->emptySpace() < idArray.size()) {
	msgType = EXCEED_CHAT_CAP;
	delete chat;
	return nullptr;
    }
    
    chat->setAddress(serv->getAddress());
    chat->setPortNum(serv->getPortNum());
    
    chat->insertUser(idArray);
        
    // Insert new chat into sub server
    serv->insertChat(*chat);
    msgType = CHAT_INFO;
    return chat;
}

void ChatServer::addUserToChat(Chat &chat, const vector<UserId> &idArray, 
			       MESSAGE_TYPE &msgType) {    
    if (chat.emptySpace() < idArray.size()) {
	msgType = EXCEED_CHAT_CAP;
	return;
    }        

    chat.insertUser(idArray);
    msgType = CONFIRM;    
}

void ChatServer::quitChat(UserInfo &user, Chat &chat, MESSAGE_TYPE &msgType) {
    if (!chat.quitChat(user.id, msgType)) {
	msgType = USER_NOT_IN_CHAT;
    }
}

SubServer* ChatServer::findSubServer() {
    // Find a suitable sub server
    list<SubServer *>::iterator it;
    for (it = _subServerList.begin(); it != _subServerList.end(); it ++) {
	if ((*it)->emptySpace() >= 1) {
	    break;
	}
    }

    SubServer *server = nullptr;
    
    if (it != _subServerList.end()) {
	// Found one
	server = *it;
    }
    else {
	// Didn't found, carete a new server
	server = createNewSubServer();
	_subServerList.push_front(server);
    }
    return server;
}

SubServer* ChatServer::createNewSubServer() {
    SubServer *server;
    try {
	server = new SubServer();
    }
    catch (string e) {
	cout << "An exception has been thrown: " << e << endl;
	if (server) {
	    delete server;
	}
	return nullptr;
    }
#ifdef DEBUG
    cout << "============" << endl;
    cout << "Create server: " << server->getPortNum() << endl;
#endif
    // Start sub server thread
    thread *t = new thread(&ChatServer::startSubServerThread, this, server);
    server->setThread(t);
    return server;
}

void ChatServer::sendToOthers(BYTE *buf, size_t len, LuxSocket *sock, 
			      Chat &chat, UserId &senderId) {
    // reader
    // get upgrade access
    boost::upgrade_lock<boost::shared_mutex> lock(chat.getReaderMutex());

    for (set<UserId>::const_iterator it = chat.getList().begin();
	 it != chat.getList().end();
	 it ++) {
	UserInfo *user = findUser(*it);
	if (user != nullptr) {
	    if (user->isOnline && !equalId(senderId, user->id)) {
		sock->send(buf, len, &(user->addr));
	    }
	}
    }    
}

void ChatServer::sendToAll(BYTE *buf, size_t len, LuxSocket *sock, Chat &chat) {
    // reader
    // get upgrade access
    boost::upgrade_lock<boost::shared_mutex> lock(chat.getReaderMutex());

    for (set<UserId>::const_iterator it = chat.getList().begin();
	 it != chat.getList().end();
	 it ++) {
	UserInfo *user = findUser(*it);
	if (user != nullptr) {
	    if (user->isOnline) {
		sock->send(buf, len, &(user->addr));
	    }
	}
    }
}

void ChatServer::mainRequestHandler(BYTE *buf, size_t len, 
				    sockaddr_in *tmpAddr) {
    // Parse message and handle request
    sockaddr_in cliAddr(*tmpAddr);
    delete tmpAddr;
    MsgId msgId;
    UserId senderId;
    REQUEST_TYPE reqType;
    MESSAGE_TYPE msgType;
        
    ChatPacket packet(buf, len);

    packet.parseMessage(msgId, senderId, reqType, msgType);
    
    cout << "Receive message from " << senderId << endl;
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
	    _mainSock->send(packet.getData(), packet.getLen(), &cliAddr);
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
	    cout << "IP: " << inet_ntoa(cliAddr.sin_addr) << "\n";
	    cout << "Ports: " << recvPort << " " << pollPort << "\n";
#endif	    
	    user = connect(senderId, cliAddr, recvPort, pollPort);
	    if (user) {
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
#ifdef DEBUG
	    cout << "Send to " << inet_ntoa(user->addr.sin_addr) << ":" << ntohs(user->addr.sin_port) << "\n";
#endif
	    _mainSock->send(packet.getData(), packet.getLen(), &(user->addr));
	    break;
	}
	case DISCONNECT: {
#ifdef DEBUG
	    cout << "User " << senderId << " disconnecting...\n"; 
#endif
	    disconnect(*user);
	    msgType = CONFIRM;
	    
	    packet.makeMessage(msgId, senderId, reqType, 
			       msgType, "Disconnect");
	    _mainSock->send(packet.getData(), packet.getLen(), &(user->addr));

	    break;
	}
	case POLLING: {
	    unsigned short recvPort, pollPort;
	    packet.parsePortNum(recvPort, pollPort);
	    user->isOnline = true;
	    updateUserPorts(*user, recvPort, pollPort);
	    msgType = CONFIRM;
	    packet.makeMessage(msgId, senderId, reqType, msgType, "OK");
	    _mainSock->send(packet.getData(), packet.getLen(), 
			   &(user->pollAddr));
	    break;
	}
	case CREATE_CHAT: {
	    vector<UserId> idArray;
	    packet.parseUserList(idArray);
	    int num = computeValidUserNumbers(idArray);
#ifdef DEBUG
	    cout << "Create chat valid user numbers: " << num << endl;
#endif	    
	    if (num < 2) {
		msgType = CREATE_CHAT_INVALID_USER;
		packet.makeMessage(msgId, senderId, reqType, msgType, 
				   "Invalid user.");
	        _mainSock->send(packet.getData(), packet.getLen(), 
				&(user->addr));
		break;
	    }
#ifdef DEBUG
	    cout << "User " << senderId << " creating a chat...\n"; 
	    cout << "User List:\n";
	    for (vector<UserId>::iterator it = idArray.begin();
		 it != idArray.end();
		 it ++) {
		cout << (*it) << endl;
	    }
#endif	    
	    Chat *chat = createChat(*user, idArray, msgType);
	    packet.makeMessage(msgId, senderId, reqType, msgType);
	    if (chat != nullptr) {
	        packet.appendMessage(chat->toBytes());
		for (vector<UserId>::iterator it = idArray.begin();
		     it != idArray.end();
		     it ++) {
		    UserInfo *user = findUser(*it);
		    if (user != nullptr) {
#ifdef DEBUG
			cout << "User Id: " << user->id << endl;
			cout << "User port: " << ntohs(user->addr.sin_port) << endl;
#endif
			_mainSock->send(packet.getData(), packet.getLen(), 
				       &(user->addr));
		    }
		}	
	    }
	    else {
	        _mainSock->send(packet.getData(), packet.getLen(), 
				&(user->addr));
	    }
	    break;
	}
	case TESTER_LOBBY: {
	    vector<UserId> tester{senderId};
	    Chat *lobby = getTesterLobby();
	    if (lobby) {
		addUserToChat(*lobby, tester, msgType);
		packet.makeMessage(msgId, senderId, reqType, msgType);
		packet.appendMessage(lobby->toBytes());
		if (msgType == CONFIRM) {
		    sendToAll(packet.getData(), packet.getLen(), _mainSock, *lobby);
		}
		else {
		    _mainSock->send(packet.getData(), packet.getLen(), &(user->addr));
		}
	    }
	    else {
		msgType = CHAT_NOT_EXIST;
		packet.makeMessage(msgId, senderId, TESTER_LOBBY, msgType);
		_mainSock->send(packet.getData(), packet.getLen(),
				&(user->addr));
	    }
	    break;
	}
	default: {
	    break;
	}
    }
}


void ChatServer::chatRequestHandler(BYTE *buf, size_t len, sockaddr_in *tmpAddr, 
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
	return;
    }
	
    ChatId chatId;
    packet.parseChatId(chatId);
    Chat *chat = serv->getChat(chatId);

    if (chat == nullptr) {
	msgType = CHAT_NOT_EXIST;
	packet.makeMessage(msgId, senderId, reqType,
			   msgType, "Chat does not exist.");
	sock->send(packet.getData(), packet.getLen(), &(user->addr));
	return;
    }
    switch (reqType) {
	case QUIT_CHAT: {
#ifdef DEBUG
	    cout << "Quit chat: "<< user->id << endl;
#endif

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
	    vector<UserId> idArray;
	    packet.parseAddUserList(idArray);
	    addUserToChat(*chat, idArray, msgType);
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
}

void ChatServer::startSubServerThread(SubServer *serv) {
    LuxSocket *sock = serv->getSocket();
    struct sockaddr_in cliAddr;	
    BYTE *buf = new BYTE[BUFSIZE];
    while (1) {	    
	int n = sock->receive(buf, BUFSIZE, &cliAddr);
	BYTE *tmpBuf = new BYTE[n];
	memcpy(tmpBuf, buf, n);
	sockaddr_in *tmpAddr = new sockaddr_in(cliAddr);
	thread(&ChatServer::chatRequestHandler, this, 
	       tmpBuf, n, tmpAddr, serv).detach();
    }
}

void ChatServer::updateAll() {
    boost::posix_time::seconds secTime(_updateTime);    
    while (1) {
	boost::this_thread::sleep(secTime);
	thread(&ChatServer::updateUserPool, this).detach();
	thread(&ChatServer::updateSubServer, this).detach();
    }
}

void ChatServer::updateUserPool() {
    if (_updatingUserPool) {
	return;
    }
    IndicatorHelper indicate(_updatingUserPool);
    // Writer 
    // access
    boost::upgrade_lock<boost::shared_mutex> lock(_userPoolMutex);
    // exclusive
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
    
    map<UserId, UserInfo *>::iterator usrMapIt =  _userPool.begin(); 
    while (usrMapIt != _userPool.end()) {
	if (usrMapIt->second->isOnline) {
	    ++usrMapIt;
	}
	else {
	    delete usrMapIt->second;
	    _userPool.erase(usrMapIt++);
	}	
    }
}
    

void ChatServer::updateUserPorts(UserInfo &user, unsigned short recvPort, 
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

void ChatServer::updateSubServer() {
    // Exclusive lock
    std::lock_guard<std::mutex> lock(_subServerMutex);
    
    list<SubServer *>::iterator it = _subServerList.begin();
    while (it != _subServerList.end()) {
	updateChats(*(*it));
	if ((*it)->isEmpty()) {
	    delete *it;
	    _subServerList.erase(it++);
	}
	else {
	    ++it;
	}
    }
    
}

void ChatServer::updateChats(SubServer &subServ) {
    if (_updatingChats) {
	return;
    }
    // sets _updatingChats back to false when exit scope
    IndicatorHelper indicate(_updatingChats);
    // writer 
    // get upgrade access
    boost::upgrade_lock<boost::shared_mutex> lock(subServ.getChatPoolMutex());
    // get exclusive access
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    map<ChatId, Chat*>& chatPool = subServ.getChats();
    map<ChatId, Chat*>::iterator chatMapIt = chatPool.begin();
    // Iterating all chats 
    while (chatMapIt != chatPool.end()) {
	// Check all users in each chat
	Chat *chat = chatMapIt->second;
	if (chat->isEmpty()) {
	    // If no one is in chat, remove chat
	    delete chat;
	    subServ.eraseChat(chatMapIt);
	    chatMapIt++;
	}
	else {
	    ++chatMapIt;
	}
    }
}    

UserInfo* ChatServer::findUser(const UserId &id) {
    // Reader 
    boost::upgrade_lock<boost::shared_mutex> lock(_userPoolMutex);
    
    map<UserId, UserInfo*>::iterator it = _userPool.find(id);
    if (it == _userPool.end()) {
	return nullptr;
    }
    return it->second;
}

bool ChatServer::verifyUser(UserInfo *userPtr, sockaddr_in &cliAddr) {
    if (userPtr == nullptr) {
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

int ChatServer::sockAddrCmp(const sockaddr_in &a, const sockaddr_in &b) {
    if (a.sin_family != b.sin_family) {
	return a.sin_family - b.sin_family;
    }
    if (a.sin_family == AF_INET) {
	// IPV4
	return a.sin_addr.s_addr - b.sin_addr.s_addr;
    }
    return -1;
}
    
int ChatServer::computeValidUserNumbers(const vector<UserId> &idArray) {
    // reader
    boost::upgrade_lock<boost::shared_mutex> lock(_userPoolMutex);
    
    int count = 0;
    for (vector<UserId>::const_iterator it = idArray.begin();
	 it != idArray.end(); it++) {
	map<UserId, UserInfo*>::iterator userIt = _userPool.find(*it);
	if (userIt != _userPool.end()) {
	    count++;
	}
    }
    return count;
}

bool ChatServer::equalId(const UserId &id0, const UserId &id1) {
    return (id0.compare(id1) == 0);
}

Chat* ChatServer::getTesterLobby() {
    std::lock_guard<std::mutex> lock(_subServerMutex);

    if (_subServerList.begin() != _subServerList.end()) {
	return (*_subServerList.begin())->getChat(0);
    }
    return nullptr;
}


ChatServer::~ChatServer() {
    delete _mainSock;
    for (map<UserId, UserInfo*>::iterator it = _userPool.begin();
	 it != _userPool.end();
	 it ++) {
	delete it->second;	
    }
    for (list<SubServer *>::iterator it = _subServerList.begin();
	 it != _subServerList.end();
	 it ++) {
	delete *it;	
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
	cout << "usage: " << argv[0] << " <port>" << endl;
	exit(0);
    }
    unsigned short port = atoi(argv[1]); 
    ChatServer server(port);
    signal(SIGINT, sigint_handler);

    server.run();
    
    return 0;
}
