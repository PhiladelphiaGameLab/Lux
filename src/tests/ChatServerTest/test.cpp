#include <assert.h>
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

class Tester {
public:
    UserId id;
    unsigned short recv;
    unsigned short poll;
    sockaddr_in addr;
    LuxSocket *sock;

    Tester(UserId id) : id(id) {
	sock = new LuxSocket(0);
	recv = sock->getPortNum();
	poll = recv;
    }
};

Tester user0("A0");

const unsigned short SERVER_PORT = 3000;

sockaddr_in serverAddr;

MsgId messageId = 0;


void initServerSock() {
    string hostname("localhost");
    struct hostent *server;
    server = gethostbyname(hostname.c_str());    
    if (server == NULL) {
	cout << "ERROR, no such host as " << hostname << "\n";
	exit(0);
    }
    bzero((char *) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serverAddr.sin_addr.s_addr, server->h_length);
    serverAddr.sin_port = htons(SERVER_PORT);
}

void testConnect(Tester &user) {
    cout << "-----------------------------------------------------" << endl;
    cout << " test connect to server" << endl;
    cout << "-----------------------------------------------------\n" << endl;

    BYTE *buf = new BYTE[BUFSIZE];
    
    MsgId msgId0 = messageId++;

    ChatPacket packet(buf, BUFSIZE);
    packet.makeMessage(msgId0, user.id, CONNECT, PORTS);
    packet.appendMessage((BYTE *)&user.recv, sizeof(user.recv));
    packet.appendMessage((BYTE *)&user.poll, sizeof(user.poll));
    
    user.sock->send(packet.getData(), packet.getLen(), &serverAddr);
    
    int n = user.sock->receive(buf, BUFSIZE, nullptr);
    
    MsgId msgId;
    UserId senderId;
    REQUEST_TYPE reqType;
    MESSAGE_TYPE msgType;

    packet.parseMessage(msgId, senderId, reqType, msgType);

    assert(msgId == msgId0);
    assert(senderId == user.id);
    assert(reqType == CONNECT);
    assert(msgType == CONFIRM);    
}

void testDisconnect(Tester &user) {
    cout << "-----------------------------------------------------" << endl;
    cout << " test disconnect to server" << endl;
    cout << "-----------------------------------------------------\n" << endl;

    BYTE *buf = new BYTE[BUFSIZE];
    
    MsgId msgId0 = messageId++;

    ChatPacket packet(buf, BUFSIZE);
    packet.makeMessage(msgId0, user.id, DISCONNECT, PORTS);
    packet.appendMessage((BYTE *)&user.recv, sizeof(user.recv));
    packet.appendMessage((BYTE *)&user.poll, sizeof(user.poll));
    
    user.sock->send(packet.getData(), packet.getLen(), &serverAddr);

    int n = user.sock->receive(buf, BUFSIZE, nullptr);

    MsgId msgId;
    UserId senderId;
    REQUEST_TYPE reqType;
    MESSAGE_TYPE msgType;

    packet.parseMessage(msgId, senderId, reqType, msgType);

    assert(msgId == msgId0);
    assert(senderId == user.id);
    assert(reqType == DISCONNECT);
    assert(msgType == CONFIRM);    

}

int main() {
    initServerSock();
    while (1) {
	testConnect(user0);
	testDisconnect(user0);
	//boost::posix_time::milliseconds secTime(10);
	//boost::this_thread::sleep(secTime); 
    }
}
