#include "luxSocket.h"
#include <string>

using namespace std::string;
using namespace std::vector;

static const int BUFSIZE = 1 << 10;

typedef int UserId; // Can be changed later to something like EUID
typedef short ChatId;

struct userInfo {
    UserId id;
    string name; // User name shown on client side
    bool isOnline; // If user is currently online
    sockaddr_in addr; // User address, including ip, port and other data
};

// Structure for each chat, contains chat id and users in the chat
struct chatInfo {
    ChatId id;
    vector<UserId> userList; // Stores user id instead of full user information.
};

// Stores all online user information here
vector<userInfo> userPool;

// Stores all chats in the list
vector <*chatInfo> chatList;

// Creates chat for users in the list
// Sends back the chat data so that all clients and create coressponding chat box
void createChat(vector<UserId> idList);

// Make chat info into a string
void makeChatInfo(ChatId chatId, char *chatInfo);

// Sends message to all users expect the sender in this chat
// Need to confirm with  sender
void sendMessage(UserId userId, ChatId chatId, char *message);

// Send message to a client
void sendMessage(UserId userId, char *message);

// Update chats in chat list
// Check if users are still online, if not remove from list
// Check if chat is still valid
void updateChatList();

// Update users in user pool
// Gets user information from social network server and updates users status
void updateUserPool();

// Helper functions

// Returns a new chat id
ChatId getNewChatId();

int main() {
    unsigned short port = 8000;
    socketLibrary::LuxSocket *udpSocket = new socketLibrary::LuxSocket(port);
    struct sockaddr_in clientAddr;        
    
    while (1) {
	// Receive data from clients and do whatever need to do.
    }
}

void createChat(vector<UserId> idList) {
    chatInfo *chat = new chatInfo();
    
    chat->id = getNewChatId();
    
    for (vecotr<UserId>::iterator it = idList.begin();
	 it != idList.end();
	 it ++) {
	chat->userList.push_back(*it);
    }
    
    chatList.push_back(chat);
    
    
}

void makeChatInfo(ChatId chatId, char *chatInfo) {
    
}

ChatId getNewChatId() {
    static ChatId id = 0;
    return id++;
}
