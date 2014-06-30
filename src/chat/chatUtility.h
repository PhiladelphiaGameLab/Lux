#ifndef CHAT_UTILITY_H
#define CHAT_UTILITY_H
#include <string>
#include <vector>
#include <iostream>

namespace chat {

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

    const int EUID_LEN = 2; // EUID length, not determined yet

    // Maximum size of UDP packet
    // 0xffff - (sizeof(IP Header) + sizeof(UDP Header)) = 65535-(20+8) = 65507
    const int BUFSIZE = 65507;

    typedef std::string UserId; // EUID
    typedef unsigned int ChatId;
    typedef unsigned short SubServerId;
    typedef unsigned int MsgId;
    typedef unsigned char BYTE;

    const int HEADER_LEN = sizeof(MsgId) + EUID_LEN + sizeof(BYTE) * 2;
    
    class ChatPacket {
// -----------------------------------------------------------------------------
// Packet format:
//    4 byte     xx* byte      1 byte        1 byte          message/parameters 
//  Message ID  sender EUID  request type  message type    ....................
// -----------------------------------------------------------------------------
// Messages from users:
// -----------------------------------------------------------------------------
// Message/Parameters format:
// USER_LIST :     4 byte         xx* byte xx* byte 
//               user number    EUID0   EUID1    ....
// CHAT_ID:  4 byte
//           chatId
// PORTS:   2 byte           2 byte
//        receiving   polling receiving
// TEXT_MSG,   
// AUDIO_MSG,
// VIDEO_MSG :   conetnt
//               ...................
// -----------------------------------------------------------------------------
// Messages from server:
// -----------------------------------------------------------------------------
// Message/Parameters format:
// CHAT_INFO :  2byte  4 bype   4 byte    4 byte      xx* byte   xx* byte
//               port  chatID  capacity  user number   EUID0      EUID1   ....
// PORTS:   2 byte           2 byte
//        receiving   polling receiving
// TEXT_MSG,
// AUDIO_MSG,
// VIDEO_MSG : ...................
// -----------------------------------------------------------------------------
// xx: length of EUID (not determined yet)
//

	public:	
	static int parseMessage(BYTE *buf, size_t len,
				MsgId &msgId, UserId &senderId, 
				REQUEST_TYPE &reqType, 
				MESSAGE_TYPE &msgType);

	static int parseUserList(BYTE *message, std::vector<UserId> &idList);
	static int parsePortNum(BYTE *message, unsigned short &recvPort, 
				unsigned short &pollPort);
	static int parseChatId(BYTE *message, ChatId &id);
	
	static size_t makeMessage(BYTE *buf, MsgId &msgId, UserId &senderId, 
			       BYTE reqType, BYTE msgType, 
			       const char *message);

	//static int parseChatInfo(char *message, int );
	
	private:
    };

    int ChatPacket::parseMessage(BYTE *buf, size_t len,
				 MsgId &msgId, UserId &senderId, 
				 REQUEST_TYPE &reqType, 
				 MESSAGE_TYPE &msgType) {
	// TODO: Error handling 
	int p = 0;
	for (int i = 0; i < sizeof(MsgId); i++) {
	    *((char*)&msgId + i) = buf[p++];	    
	}
	senderId.clear();
	for (int i = 0; i < EUID_LEN; i++) {
	    senderId.push_back(buf[p++]);
	}
	reqType = (REQUEST_TYPE)buf[p++];
	msgType = (MESSAGE_TYPE)buf[p++];

	return 1;	
    }
        
    int ChatPacket::parseUserList(BYTE *message, std::vector<UserId> &list) {
	int count = 0;
	int p = HEADER_LEN;
	for (int i = 0; i < sizeof(count); i++) {
	    *((BYTE*)&count + i) = message[p++];
	}
	for (int i = 0; i < count; i++) {
	    UserId tmp;
	    for (int j = 0; i < EUID_LEN; j++) {
		tmp.push_back(message[p++]);
	    }
	    list.push_back(tmp);
	}
	
	return 1;
    }

    int ChatPacket::parsePortNum(BYTE *message, 
				 unsigned short &recvPort, 
				 unsigned short &pollPort) {
	int p = HEADER_LEN;

	for (int i = 0; i < sizeof(recvPort); i++) {
	    *((BYTE*)&recvPort + i) = message[p++];
	}

	for (int i = 0; i < sizeof(pollPort); i++) {
	    *((BYTE*)&pollPort + i) = message[p++];
	}
	return 1;
    }
    
    int ChatPacket::parseChatId(BYTE *message, ChatId &id) {
	
	for (int i = 0; i < sizeof(id); i++) {
	    *((BYTE*)&id + i) = message[i + HEADER_LEN];
	}
	return 1;
    }

    size_t ChatPacket::makeMessage(BYTE *buf, MsgId &msgId, UserId &senderId, 
				BYTE reqType, BYTE msgType, 
				const char *message) {
	int p = HEADER_LEN;
	for (int i = 0; i < sizeof(MsgId); i++) {
	    buf[p++] = *((char*)&msgId + i);
	}
	for (int i = 0; i < EUID_LEN; i++) {
	    buf[p++] = senderId[i];
	}
	buf[p++] = reqType;
	buf[p++] = msgType;
	memcpy(buf + p, message, strlen(message));
	p += strlen(message);
	return p;
    }
    /*
    int ChatPacket::parseChatInfo() {
	return 1;
    }
    */

}
#endif 
//CHAT_UTILITY_H
