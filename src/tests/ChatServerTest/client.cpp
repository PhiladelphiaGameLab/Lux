/*
 * single chat client - can only in one chat at a time
 *
 */

#include <iostream>
#include <string>
#include <cstring>
#include "luxSocket.h"
#include "chatUtility.h"
#include <boost/thread/thread.hpp>

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

struct ChatRoom {
    struct sockaddr_in addr;
    ChatId id;
};

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
	cin >> f;
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
		quitCurChat();
		break;
	    case 5: {
		string tmp;
		cin >> tmp;
		sendMessage(tmp);
		continue;
		break;
	    }
	    case 6:
		quit();
		break;
	    default:
		cout << "No such funtion.\n";
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
    cout << "Select a function:\n";
    cout << "Connect to server: 1\n";
    cout << "Disconnect: 2\n";
    cout << "Create a chat. 3 number id0 id1 id2 ...\n";
    cout << "Quit chat: 4 \n";
    cout << "Send text message: 5 message....\n";
    cout << "Quit application: 6\n";
}

void connect() {
    buf[p++] = (unsigned char)CONNECT;
    buf[p++] = (unsigned char)PORTS;
    for (int i = 0; i < sizeof(clientPort); i++) {
	buf[p++] = *((unsigned char*)&clientPort + i);
    }
    for (int i = 0; i < sizeof(pollingPort); i++) {
	buf[p++] = *((unsigned char*)&pollingPort + i);
    }
}

void disconnect() {
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
    cout << "Chat ip :" << chatRoom->addr.sin_addr.s_addr << endl;
    cout << "Main ip :" << serverAddr.sin_addr.s_addr << endl;
    cout << "Port :" << chatRoom->addr.sin_port << endl;
    int n = sendto(sockfd, buf, p, 0, (struct sockaddr *)&chatRoom->addr, 
		   sizeof(serverAddr));
    if (n < 0) {
	cout << "Error in sendto\n";
	exit(0);
    }

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
    cout << senderId << endl;
    cout << reqType << endl;
    cout << msgType << endl;
    */
    
    if (reqType == CREATE_CHAT && msgType == CHAT_INFO) {
	unsigned short port;
	ChatId id;
	int cap;
	int count;
	vector<UserId> users;
	pack.parseChatInfo(port, id, cap, count, users);
	chatRoom = new ChatRoom();
	chatRoom->addr = serverAddr;
	chatRoom->addr.sin_port = port;
	chatRoom->id = id;
	/*
	cout << "ChatId " << id << endl;
	cout << "Number: " << count << " / " << cap << endl;
	cout << "Port: " << port << endl;
	cout << "Uesrs: ";
	for (int i = 0; i < count; i++) {
	    cout << users[i] << " ";
	}
	cout << endl;
	*/	
    }
    else if (reqType == SEND_MESSAGE && msgType == TEXT_MSG) {
	ChatId id;
	string text;
	pack.parseTextMsg(id, text);
	cout << "Chat Room Id: " << id << endl;
	cout << "From: " << senderId << endl;
	cout << "Message:" << text << endl;
    }
}

void startReceiving(LuxSocket *sock) {
    int n = 0;
    while (1) {
	n = sock->receive(buf, BUFSIZE, &serverAddr);
	cout << "receive" << n << endl;
	print(buf, n);
    }
}
