/*
 * single chat client - can only in one chat at a time
 *
 */
#include <unistd.h>
#include <term.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include "luxSocket.h"
#include "chatUtility.h"
#include <boost/thread/thread.hpp>
#include <mutex>

using namespace std;
using namespace socketlibrary;
using namespace chat;
using boost::thread;

unsigned char buf[BUFSIZE];
unsigned int messageId = 0;
string EUID("");
int p;
unsigned short clientPort = 8000;
unsigned short pollingPort = 8000;

unsigned short serverPort = 3000;
struct sockaddr_in serverAddr;
int sockfd;

const int WIDTH = 40;

struct ChatRoom {
    struct sockaddr_in addr;
    ChatId id;
};

enum CHAT_STATE {
    MENU,
    IN_CHAT    
};

enum ALIGN {
    FLOAT_LEFT,
    FLOAT_RIGHT
};

CHAT_STATE chatState = MENU;

ChatRoom *chatRoom;

void menu();
void connect();
void disconnect();
void createChat();
void quitCurChat();
void sendMessage(const string &msg);
void quit();
void print(unsigned char *buf, int n);
void startReceiving(LuxSocket *sock);
void clearScreen();
void saveMessage(const string& msg, ALIGN a);

vector<string> display;
vector<string> history;
vector<int> mine;
stringstream ss;

std::mutex printMutex;

int main(int argc, char ** argv) {
    char* hostname = "localhost";
    struct hostent *server;
    
    if (argc != 3) {
	cout << "usage: " << argv[0] << " <port> <EUID(2byte)>" << endl;
	exit(0);
    }
    
    clientPort = pollingPort = atoi(argv[1]);
    
    EUID = argv[2];
    
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    // gethostbyname: get the server's DNS entry
    server = gethostbyname(hostname);    
    if (server == NULL) {
	cout << "ERROR, no such host as " << hostname << "\n";
	exit(0);
    }

    // build the server's Internet address
    bzero((char *) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serverAddr.sin_addr.s_addr, server->h_length);
    serverAddr.sin_port = htons(serverPort);

    // client receving socket
    LuxSocket client(clientPort);

    cout << "Chat client started.\n" << endl;
    
    LuxSocket *tmpSock = &client;

    thread(startReceiving, tmpSock).detach();

    int f, n;
    string input;
    while(1) {
	menu();
	memset(buf, 0, BUFSIZE);
	// Append message id
	messageId ++;
	p = 0;
	for (int i = 0; i < sizeof(messageId); i++) {
	    buf[p++] = *((unsigned char*)&messageId + i);
	}
	for (int i = 0; i < EUID.size(); i++) {
	    buf[p++] = EUID[i];
	}	
	cin >> input;
	if (input[0] >= '0' && input[0] <= '9') {
	    f = input[0] - '0';
	}
	else {
	    continue;
	}
	if (chatState == MENU) {
	    switch (f) {
		case 1:
		    connect();
		    break;
		case 2:
		    disconnect();
		    break;
		case 3:
		    createChat();
		    break;
		case 4:
		    quit();
		    break;
		default:
		    continue;
	    }
	}
	else {
	    switch(f) {
		case 0:
		    chatState = MENU;
		    break;
		default: {
		    char msg[1024];
		    cin.getline(msg, 1024);
		    sendMessage(msg);
		}
	    }
	    continue;
	}
	//print(buf, p);
	n = sendto(sockfd, buf, p, 0, (struct sockaddr *)&serverAddr, 
		   sizeof(serverAddr));
	if (n < 0) {
	    cout << "Error in sendto\n";
	    exit(0);
	}
    }
}

void menu() {
    std::lock_guard<std::mutex> lock(printMutex);
    clearScreen();
    if (chatState == MENU) {
	cout << "==================Menu==================\n";
	cout << "Connect to server: 1\n";
	cout << "Disconnect: 2\n";
	cout << "Create a chat. 3 number id0 id1 id2 ...\n";
	cout << "Quit application: 4\n";
	cout << "========================================\n";
    }
    else {
	cout << "==================Chat==================\n";
	cout << "Quit chat 0.\n";
	cout << "Send text message: 1 message(one line)\n";
	cout << "========================================\n";
    }

    for (auto it = display.begin(); it != display.end(); it++) {
	cout << *it;
    }

    if (chatState == IN_CHAT) {
	string padding[2];
	int boundary = WIDTH / 2;
	for (int i = 0; i < boundary - 2; i++) {
	    padding[1].push_back(' ');
	}
	padding[1] += " | ";
	for (int i = 0; i < history.size(); i++) {
	    cout << history[i];
	    /*
	    string cur = history[i];
	    int k = cur.find_first_of('\n');
	    if (k) {	
		cout << padding[mine[i]];
		cout << cur.substr(0, k + 1);
		cur = cur.substr(k + 1, cur.size() - k);
	    }
	    cout << padding[mine[i]];	    
	    int cnt = 0;
	    for (int j = 0; j < cur.size(); j++) {
		if (cnt == boundary - 1 && j != cur.size() - 1) {
		    cout << " | \n";
		    cout << padding[mine[i]];
		    cnt = 0;
		}
		else {
		    cout << cur[j];
		    cnt++;
		}
	    }
	    */
	}
    }	
    
    display.clear();
}

void connect() {
    buf[p++] = (unsigned char)CONNECT;
    buf[p++] = (unsigned char)PORTS;
    ss << "Connect port: " << ntohs(serverAddr.sin_port) << endl;
    display.push_back(ss.str());
    ss.str("");
    for (int i = 0; i < sizeof(clientPort); i++) {
	buf[p++] = *((unsigned char*)&clientPort + i);
    }
    for (int i = 0; i < sizeof(pollingPort); i++) {
	buf[p++] = *((unsigned char*)&pollingPort + i);
    }
}

void disconnect() {
    ss << "Disconnect port: " << ntohs(serverAddr.sin_port) << endl;
    display.push_back(ss.str());
    ss.str("");
    buf[p++] = (unsigned char)DISCONNECT;
    buf[p++] = (unsigned char)PORTS;
    for (int i = 0; i < sizeof(clientPort); i++) {
	buf[p++] = *((char*)&clientPort + i);
    }
    for (int i = 0; i < sizeof(pollingPort); i++) {
	buf[p++] = *((char*)&pollingPort + i);
    }
}

void createChat() {
    cout << "Creating chat" << endl;
    buf[p++] = CREATE_CHAT;
    buf[p++] = USER_LIST;
    int n;
    cin >> n;
    vector<string> list;
    list.push_back(EUID);
    string id;
    while(n--) {
	cin >> id;
	list.push_back(id);
    }
    int size = list.size();
    for (int i = 0; i < sizeof(size); i++) {
	buf[p++] = *((BYTE*)&size + i);
    }
    for (vector<string>::iterator it = list.begin();
	 it != list.end();
	 it ++) {
	for (int i = 0; i < (*it).size(); i++) {
	    buf[p++] = (*it)[i];
	}
    }
}

void quitCurChat() {
    
}

void sendMessage(const string &msg) {
    buf[p++] = (BYTE)SEND_MESSAGE;
    buf[p++] = (BYTE)TEXT_MSG;
    for (int i = 0; i < sizeof(ChatId); i++) {
	buf[p++] = *((BYTE*)&(chatRoom->id) + i);
    }
    for (int i = 0; i < msg.size(); i++) {
	buf[p++] = msg[i];
    }    
    /*
    ss << "Chat ip :" << chatRoom->addr.sin_addr.s_addr << endl;
    display.push_back(ss.str());
    ss.str("");
    ss << "Main ip :" << serverAddr.sin_addr.s_addr << endl;
    display.push_back(ss.str());
    ss.str("");
    ss << "Port :" << chatRoom->addr.sin_port << endl;
    display.push_back(ss.str());
    ss.str("");
    */
    int n = sendto(sockfd, buf, p, 0, (struct sockaddr *)&(chatRoom->addr), 
		   sizeof(serverAddr));
    if (n < 0) {
	cout << "Error in sendto\n";
	exit(0);
    }
    
    ss << "me:\n" << msg << endl << endl;
    saveMessage(ss.str(), FLOAT_RIGHT);
    ss.str("");
}

void quit() {
    exit(0);
}

void print(unsigned char *buf, int n) {
    /*
    cout << "len: " << n << " : ";
    for (int i = 0; i < n; i++) {
	cout << buf[i];
    } 
    cout << endl;
    */   
    int len = n;
    MsgId msgId;
    UserId senderId;
    REQUEST_TYPE reqType;
    MESSAGE_TYPE msgType;
    ChatPacket pack(buf, n);
    pack.parseMessage(msgId, senderId, reqType, msgType);
    /*
    ss << senderId << endl;
    ss << reqType << endl;
    ss << msgType << endl;
    display.push_back(ss.str());
    ss.str("");
    */
    if (reqType == CONNECT && msgType == CONFIRM) {
	ss << "Connected" << endl;
	display.push_back(ss.str());
	ss.str("");	
    }
    
    if (reqType == DISCONNECT && msgType == CONFIRM) {
	ss << "Disconnected" << endl;
	display.push_back(ss.str());
	ss.str("");	
    }

    if (reqType == CREATE_CHAT && msgType == CHAT_INFO) {
	unsigned short port;
	ChatId id;
	int cap;
	int count;
	vector<UserId> users;
	pack.parseChatInfo(port, id, cap, count, users);
	chatRoom = new ChatRoom();
	chatRoom->addr = serverAddr;
	chatRoom->addr.sin_port = htons(port);
	chatRoom->id = id;

	ss << "ChatId " << id << endl;
	ss << "Number: " << count << " / " << cap << endl;
	ss << "Port: " << port << endl;
	ss << "Uesrs: ";
	for (int i = 0; i < count; i++) {
	    ss << users[i] << " " << endl;
	}
	display.push_back(ss.str());
	ss.str("");
	chatState = IN_CHAT;
    }
    else if (reqType == SEND_MESSAGE && msgType == TEXT_MSG) {
	ChatId id;
	string text;
	pack.parseTextMsg(id, text);
	ss << senderId << " says:"<< endl;
	ss << text << endl << endl;
	saveMessage(ss.str(), FLOAT_LEFT);
	ss.str("");
    }
    menu();
}

void startReceiving(LuxSocket *sock) {
    int n = 0;
    BYTE *buf = new BYTE[BUFSIZE];
    while (1) {
	sockaddr_in tmpAddr;
	n = sock->receive(buf, BUFSIZE, &tmpAddr);
	//ss << "receive" << n << endl;
	//display.push_back(ss.str());
	//ss.str("");
	BYTE *tmp = new BYTE[n];
	memcpy(tmp, buf, n);
	print(tmp, n);
    }
}

void clearScreen() {
    if (!cur_term) {
	int result;
	setupterm( NULL, STDOUT_FILENO, &result );
	if (result <= 0) return;
    }
    putp( tigetstr( "clear" ) );
}

void saveMessage(const string& msg, ALIGN a) {
    string tmp;
    string padding;
    int boundary = WIDTH / 2 - 1;
    
    if (a == FLOAT_RIGHT) {
	for (int i = 0; i < boundary; i++) {
	    padding.push_back(' ');
	}
	padding.push_back('|');
	padding.push_back(' ');
    }
    int i = 0;
    while (msg[i] == ' ' && i < msg.size() - 1) {
	i ++;
    }
    int cnt = 0;
    tmp += padding;
    while (i < msg.size() - 1) {
	if (msg[i] == '\n') {
	    while (cnt < boundary - 1) {
		tmp.push_back(' ');
		cnt++;
	    }
	    if (a == FLOAT_LEFT) {
		tmp.push_back(' ');
		tmp.push_back('|');
	    }
	    tmp.push_back('\n');
	    if (i < msg.size() - 2) {
		tmp += padding;		
	    }
	    cnt = 0;
	}
	else {
	    tmp.push_back(msg[i]);
	    cnt++;
	    if (cnt == boundary - 1) {
		if (msg[i + 1] == ' ') {
		    i++;
		}
		if (a == FLOAT_LEFT) {
		    tmp.push_back(' ');
		    tmp.push_back('|');
		}
		tmp.push_back('\n');
		if (i < msg.size() - 2) {
		    tmp += padding;		
		}
		cnt = 0;
	    }
	}
	i++;
    }
    history.push_back(tmp);
}
