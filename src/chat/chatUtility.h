#ifndef CHAT_UTILITY_H
#define CHAT_UTILITY_H
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

namespace chat {

    enum REQUEST_TYPE{
	CONNECT = 0,
	DISCONNECT = 1,    
	POLLING = 2,
	CREATE_CHAT = 3,
	QUIT_CHAT = 4,
	ADD_USER_TO_CHAT = 5,
	SEND_MESSAGE = 6,
	TESTER_LOBBY = 7
    };

    enum MESSAGE_TYPE {    
	NO_MORE_SERVERS = 0,
	EXCEED_CHAT_CAP = 1,
	CHAT_NOT_EXIST = 2,
	USER_NOT_IN_CHAT = 3,
	CONNECT_ERROR = 4,
	USER_LIST = 5,
	PORTS = 6,
	CHAT_ID = 7,
	TEXT_MSG = 8,
	AUDIO_MSG = 9,
	VIDEO_MSG = 10,
	CHAT_INFO = 11,
	SERVER_INFO = 12,
	RE_CONNECT = 13,
	CONFIRM = 14,
	CREATE_CHAT_INVALID_USER = 15
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


    enum COPY_TYPE{
	NOT_COPY_OLD,
	COPY_OLD
    };

    class ChatPacket {
// -----------------------------------------------------------------------------
// Packet format:
//    4 byte     xx* byte      1 byte        1 byte        message/parameters 
//  Message ID  sender EUID  request type  message type  ....................
// -----------------------------------------------------------------------------
// Messages from users:
// -----------------------------------------------------------------------------
// Message/Parameters format:
// USER_LIST :  4 byte       xx* byte xx* byte 
//             user number    EUID0   EUID1    ....
// -----------------------------------------------------------------------------
// ADD_USER_LIST :  4 byte    4 byte         xx* byte xx* byte 
//                  chatId  user number       EUID0   EUID1    ....
// -----------------------------------------------------------------------------
// CHAT_ID:  4 byte
//           chatId	
// PORTS:   2 byte           2 byte
//        receiving   polling receiving
// TEXT_MSG,   
// AUDIO_MSG,
// VIDEO_MSG :   4 byte     conetnt
//               chatId    ...................
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
	ChatPacket(BYTE *buf, size_t len) 
	    : _buf(buf), _len(len), _maxLen(len) {};
	~ChatPacket() {
	    delete[] _buf;
	};
	int parseMessage(MsgId &msgId, UserId &senderId, 
			 REQUEST_TYPE &reqType, 
			 MESSAGE_TYPE &msgType);

	int parseAddUserList(std::vector<UserId> &idList);
	int parseUserList(std::vector<UserId> &idList);
	int parseUsersAt(std::vector<UserId> &list, int offset);
	int parsePortNum(unsigned short &recvPort, 
			 unsigned short &pollPort);
	int parseChatId(ChatId &id);

	int parseChatInfo(unsigned short &port, ChatId &id, int &cap, int &cnt, 
			  std::vector<UserId> &list);
	int parseTextMsg(ChatId &id, std::string &text);
	
	size_t makeMessage(MsgId &msgId, UserId &senderId, 
			   BYTE reqType, BYTE msgType, 
			   const char *message);
	size_t makeMessage(MsgId &msgId, UserId &senderId, 
			   BYTE reqType, BYTE msgType);
	size_t appendMessage(const BYTE *msg, size_t len);
	size_t appendMessage(const std::string &msg);
	size_t appendMessage(const std::vector<BYTE> &msg);
	void keepEnoughBuf(size_t newLen, COPY_TYPE c);
	
	BYTE* getData() {
	    return _buf;
	}
	
	size_t getLen() {
	    return _len;
	}
	
	//static int parseChatInfo(char *message, int );
	
	private:
	BYTE *_buf;
	size_t _len;
	size_t _maxLen;
    };

    int ChatPacket::parseMessage(MsgId &msgId, UserId &senderId, 
				 REQUEST_TYPE &reqType, 
				 MESSAGE_TYPE &msgType) {
	// TODO: Error handling 
	int p = 0;
	for (int i = 0; i < sizeof(MsgId); i++) {
	    *((char*)&msgId + i) = _buf[p++];	    
	}
	senderId.clear();
	for (int i = 0; i < EUID_LEN; i++) {
	    senderId.push_back(_buf[p++]);
	}
	reqType = (REQUEST_TYPE)_buf[p++];
	msgType = (MESSAGE_TYPE)_buf[p++];

	return 1;	
    }
    
    int ChatPacket::parseAddUserList(std::vector<UserId> &list) {
	return parseUsersAt(list, HEADER_LEN + sizeof(ChatId));	
    }
	
        
    int ChatPacket::parseUserList(std::vector<UserId> &list) {
	return parseUsersAt(list, HEADER_LEN);
    }

    int ChatPacket::parseUsersAt(std::vector<UserId> &list, int offset) {
	int count = 0;
	int p = offset;
	for (int i = 0; i < sizeof(count); i++) {
	    *((BYTE*)&count + i) = _buf[p++];
	}
	for (int i = 0; i < count; i++) {
	    UserId tmp;
	    for (int j = 0; j < EUID_LEN; j++) {
		tmp.push_back(_buf[p++]);
	    }
	    list.push_back(tmp);
	}
	
	return 1;
    }

    int ChatPacket::parsePortNum(unsigned short &recvPort, 
				 unsigned short &pollPort) {
	int p = HEADER_LEN;

	for (int i = 0; i < sizeof(recvPort); i++) {
	    *((BYTE*)&recvPort + i) = _buf[p++];
	}

	for (int i = 0; i < sizeof(pollPort); i++) {
	    *((BYTE*)&pollPort + i) = _buf[p++];
	}
	return 1;
    }
    
    int ChatPacket::parseChatId(ChatId &id) {	
	for (int i = 0; i < sizeof(id); i++) {
	    *((BYTE*)&id + i) = _buf[i + HEADER_LEN];
	}
	return sizeof(id);
    }

    size_t ChatPacket::makeMessage(MsgId &msgId, UserId &senderId,
				   BYTE reqType, BYTE msgType, 
				   const char *message) {
	int newLen = 0;
	newLen += HEADER_LEN;
	newLen += strlen(message) + 1;
	keepEnoughBuf(newLen, NOT_COPY_OLD);
	_len = 0;
	for (int i = 0; i < sizeof(MsgId); i++) {
	    _buf[_len++] = *((BYTE*)&msgId + i);
	}
	for (int i = 0; i < EUID_LEN; i++) {
	    _buf[_len++] = senderId[i];
	}
	_buf[_len++] = reqType;
	_buf[_len++] = msgType;
	memcpy(_buf + _len, message, strlen(message));
	_len += strlen(message);
	return _len;
    }

    size_t ChatPacket::makeMessage(MsgId &msgId, UserId &senderId, 
				   BYTE reqType, BYTE msgType) {
	_len = 0;
	for (int i = 0; i < sizeof(MsgId); i++) {
	    _buf[_len++] = *((char*)&msgId + i);
	}
	for (int i = 0; i < EUID_LEN; i++) {
	    _buf[_len++] = senderId[i];
	}
	_buf[_len++] = reqType;
	_buf[_len++] = msgType;
	return _len;
    }
    
    size_t ChatPacket::appendMessage(const BYTE *msg, size_t len) {
	int newLen = _len + len;
	keepEnoughBuf(newLen, COPY_OLD);
	memcpy(_buf + _len, msg, len);
	_len += len;
	return len;
    }
    
    size_t ChatPacket::appendMessage(const std::string &msg) {	
	size_t len = msg.length();
	int newLen = _len + len;
	keepEnoughBuf(newLen, COPY_OLD);
	memcpy(_buf + _len, msg.c_str(), len);
	_len += len;
	return len;
    }

    size_t ChatPacket::appendMessage(const std::vector<BYTE> &msg) {	
	size_t len = msg.size();
	int newLen = _len + len;
	keepEnoughBuf(newLen, COPY_OLD);
	for (size_t i = 0; i < len; i++) {
	    _buf[_len++] = msg[i];
	}
	return len;
    }
    
    void ChatPacket::keepEnoughBuf(size_t newLen, COPY_TYPE c) {
	if (_maxLen < newLen) {
	    BYTE *newBuf = new BYTE[newLen];
	    if (c == COPY_OLD) {
		memcpy(newBuf, _buf, _len);
	    }	    
	    delete _buf;
	    _buf = newBuf;
	}
    }

    int ChatPacket::parseChatInfo(unsigned short &port, ChatId &id, int &cap,
				  int &cnt, std::vector<UserId> &list) {
	int p = HEADER_LEN;
	
	for (int i = 0; i < sizeof(port); i++) {
	    *((BYTE*)&port + i) = _buf[p++];	    
	}

	for (int i = 0; i < sizeof(ChatId); i++) {
	    *((BYTE*)&id + i) = _buf[p++];
	}

	for (int i = 0; i < sizeof(cap); i++) {
	    *((BYTE*)&cap + i) = _buf[p++];
	}

	for (int i = 0; i < sizeof(cnt); i++) {
	    *((BYTE*)&cnt + i) = _buf[p++];
	}
	parseUsersAt(list, p - sizeof(cnt));

	return 1;
    }
    
    int ChatPacket::parseTextMsg(ChatId &id, std::string &text) {
	
	int p = HEADER_LEN;
	p += parseChatId(id);
	
	char *tmp = new char[_maxLen - p + 1];
	memcpy(tmp, _buf + p, _maxLen - p);
	
	tmp[_maxLen - p] = '\0';

	text = tmp;
	delete tmp;
	
	return 1;
    }        

}
#endif 
//CHAT_UTILITY_H
