#include <iostream>
#include <string>
#include <cstring>
#include "luxSocket.h"

using namespace std;
using namespace socketlibrary;

enum REQUEST_TYPE{
    CONNECT,
    DISCONNECT,    
    POLLING,
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
    PORTS,
    CHAT_ID,
    TEXT_MSG,
    AUDIO_MSG,
    VIDEO_MSG,
    CHAT_INFO,
    SERVER_INFO,
    RE_CONNECT,
    CONFIRM
};

const int BUFSIZE = 65507;
unsigned char buf[BUFSIZE];
unsigned int messageId = 0;
string EUID("A0");
int p;
unsigned short clientPort = 8000;
unsigned short pollingPort = 8000;


void menu();
void connect();
void disconnect();
void createChat();
void quitCurChat();
void sendMessage();
void quit();
void print(unsigned char *buf, int n);

int main() {
    int sockfd;
    unsigned short serverPort = 3000;
    struct sockaddr_in serverAddr;
    char* hostname = "localhost";
    struct hostent *server;
    
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
    
    int f, n;

    while(1) {
	menu();
	memset(buf, 0, BUFSIZE);
	// Append message id
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
	    case 5:
		sendMessage();
		break;
	    case 6:
		quit();
		break;
	    default:
		cout << "No such funtion.\n";
	}
	print(buf, p);
	n = sendto(sockfd, buf, p, 0, (struct sockaddr *)&serverAddr, 
		   sizeof(serverAddr));
	if (n < 0) {
	    cout << "Error in sendto\n";
	    exit(0);
	}
	n = client.receive(buf, BUFSIZE, &serverAddr);
	cout << "receive" << n << endl;
	print(buf, n);
	//parseMessage();
    }
}

void menu() {
    cout << "Select a function:\n";
    cout << "Connect to server: 1\n";
    cout << "Disconnect: 2\n";
    cout << "Create a chat. 3 id0 id1 id2 ...\n";
    cout << "Quit chat: 4 \n";
    cout << "Send text message: 5 message....\n";
    cout << "Quit application: 6\n";
}

void connect() {
    buf[p++] = (unsigned char)CONNECT;
    cout << "CONNECT " << CONNECT << endl;
    cout << "CONNECT " << (unsigned char)CONNECT << " " << endl;
    cout << "CONNECT " << (REQUEST_TYPE)CONNECT << " " << endl;
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
    for (int i = 0; i < EUID.size(); i++) {
	buf[p++] = EUID[i];
    }
    string id;
    while(cin >> id) {
	for (int i = 0; i < id.size(); i++) {
	    buf[p++] = id[i];
	}
    }
    buf[p] = '\0';
}

void quitCurChat() {
    
}

void sendMessage() {
    
}

void quit() {
    
}

void print(unsigned char *buf, int n) {
    cout << "len: " << n << " : ";
    for (int i = 0; i < n; i++) {
	cout << buf[i];
    }
    cout << endl;
}
