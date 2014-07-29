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

struct ChatRoom {
    struct sockaddr_in addr;
    ChatId id;
};

sockaddr_in serverAddr;

MsgId messageId = 0;

void initServerSock(const char *hostname, const unsigned short port) {
    struct hostent *server;
    server = gethostbyname(hostname);    
    if (server == NULL) {
	cout << "ERROR, no such host as " << hostname << "\n";
	exit(0);
    }
    bzero((char *) &serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serverAddr.sin_addr.s_addr, server->h_length);
    serverAddr.sin_port = htons(port);
}

void testConnect(Tester &user0) {
    cout << "-----------------------------------------------------" << endl;
    cout << " test connect to server" << endl;
    cout << "-----------------------------------------------------\n" << endl;

    BYTE *buf = new BYTE[BUFSIZE];
    
    MsgId msgId0 = messageId++;

    ChatPacket packet(buf, BUFSIZE);
    packet.makeMessage(msgId0, user0.id, CONNECT, PORTS);
    packet.appendMessage((BYTE *)&user0.recv, sizeof(user0.recv));
    packet.appendMessage((BYTE *)&user0.poll, sizeof(user0.poll));
    
    user0.sock->send(packet.getData(), packet.getLen(), &serverAddr);
    
    BYTE *bufRecv = new BYTE[BUFSIZE];

    int n = user0.sock->receive(bufRecv, BUFSIZE, nullptr);

    ChatPacket packetRecv(bufRecv, n);

    MsgId msgId;
    UserId senderId;
    REQUEST_TYPE reqType;
    MESSAGE_TYPE msgType;    

    packetRecv.parseMessage(msgId, senderId, reqType, msgType);

    assert(msgId == msgId0);
    assert(senderId == user0.id);
    assert(reqType == CONNECT);
    assert(msgType == CONFIRM);    
}

void testDisconnect(Tester &user0) {
    cout << "-----------------------------------------------------" << endl;
    cout << " test disconnect to server" << endl;
    cout << "-----------------------------------------------------\n" << endl;

    BYTE *buf = new BYTE[BUFSIZE];
    
    MsgId msgId0 = messageId++;

    ChatPacket packet(buf, BUFSIZE);
    packet.makeMessage(msgId0, user0.id, DISCONNECT, PORTS);
    packet.appendMessage((BYTE *)&user0.recv, sizeof(user0.recv));
    packet.appendMessage((BYTE *)&user0.poll, sizeof(user0.poll));
    
    user0.sock->send(packet.getData(), packet.getLen(), &serverAddr);

    BYTE *bufRecv = new BYTE[BUFSIZE];

    int n = user0.sock->receive(bufRecv, BUFSIZE, nullptr);

    ChatPacket packetRecv(bufRecv, n);

    MsgId msgId;
    UserId senderId;
    REQUEST_TYPE reqType;
    MESSAGE_TYPE msgType;    

    packetRecv.parseMessage(msgId, senderId, reqType, msgType);

    assert(msgId == msgId0);
    assert(senderId == user0.id);
    assert(reqType == DISCONNECT);
    assert(msgType == CONFIRM);    
}

void testQuitChat(Tester user0, ChatRoom &chatRoom) {
    cout << "-----------------------------------------------------" << endl;
    cout << " test quit chat" << endl;
    cout << "-----------------------------------------------------\n" << endl;

    BYTE *buf = new BYTE[BUFSIZE];
    ChatPacket packet(buf, BUFSIZE);
    MsgId msgId0 = messageId++;
    packet.makeMessage(msgId0, user0.id, QUIT_CHAT, CHAT_ID);
    packet.appendMessage((BYTE*)&(chatRoom.id), sizeof(chatRoom.id));
    
    user0.sock->send(packet.getData(), packet.getLen(), &(chatRoom.addr));

    BYTE *bufRecv = new BYTE[BUFSIZE];

    int n = user0.sock->receive(bufRecv, BUFSIZE, nullptr);

    ChatPacket packetRecv(bufRecv, n);

    MsgId msgId;
    UserId senderId;
    REQUEST_TYPE reqType;
    MESSAGE_TYPE msgType;    

    packetRecv.parseMessage(msgId, senderId, reqType, msgType);

    assert(msgId == msgId0);
    assert(senderId == user0.id);
    assert(reqType == QUIT_CHAT);
    assert(msgType == CONFIRM);    
}

void testCreateAndQuitChat(Tester &user0, Tester &user1) {
    cout << "-----------------------------------------------------" << endl;
    cout << " test create chat" << endl;
    cout << "-----------------------------------------------------\n" << endl;
    
    testConnect(user0);
    testConnect(user1);
    
    BYTE *buf = new BYTE[BUFSIZE];
    
    MsgId msgId0 = messageId++;

    ChatPacket packet(buf, BUFSIZE);
    packet.makeMessage(msgId0, user0.id, CREATE_CHAT, USER_LIST);
    vector<UserId> userArr;
    userArr.push_back(user0.id);
    userArr.push_back(user1.id);
    packet.appendUserList(userArr);
    
    user0.sock->send(packet.getData(), packet.getLen(), &serverAddr);            

    BYTE *bufRecv = new BYTE[BUFSIZE];

    int n = user0.sock->receive(bufRecv, BUFSIZE, nullptr);

    ChatPacket packetRecv(bufRecv, n);

    MsgId msgId;
    UserId senderId;
    REQUEST_TYPE reqType;
    MESSAGE_TYPE msgType;    

    packetRecv.parseMessage(msgId, senderId, reqType, msgType);

    assert(msgId == msgId0);
    assert(senderId == user0.id);
    assert(reqType == CREATE_CHAT);
    assert(msgType == CHAT_INFO);    
    
    unsigned short port;
    ChatId id;
    int cap;
    int count;
    vector<UserId> users;

    packetRecv.parseChatInfo(port, id, cap, count, users);
    
    ChatRoom chatRoom;
    chatRoom.id = id;
    chatRoom.addr = serverAddr;
    chatRoom.addr.sin_port = htons(port);

    testQuitChat(user0, chatRoom);
    // skip message create chat
    user1.sock->receive(bufRecv, BUFSIZE, nullptr);
    // skip message user0 quit chat
    user1.sock->receive(bufRecv, BUFSIZE, nullptr);
    testQuitChat(user1, chatRoom);
}

int main(int argc, char **argv) {
    if (argc != 3) {
	cout << "usage: " << argv[0] << " <hostname> <hostPort>" << endl;
	exit(0);
    }
    initServerSock(argv[1], atoi(argv[2]));

    Tester user0("A000000000");
    
    for (int i = 0; i < 2; i++) {
	testConnect(user0);
	testDisconnect(user0);
	//boost::posix_time::milliseconds secTime(10);
	//boost::this_thread::sleep(secTime); 
    }
    
    Tester user1("A000000001");
    Tester user2("A000000002");

    for (int i = 0; i < 10; i++) {
	testCreateAndQuitChat(user1, user2);
    }    
    
    return 0;
}
