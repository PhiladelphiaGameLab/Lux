#ifndef SENDUPDATE_H
#define SENDUPDATE_H

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include <string>
#include <cstdlib>
#include <iostream>

#include <mutex>

#include <netinet/in.h>

#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"
#include "mongo/db/json.h"

#include "HMBL.h"
#include "luxSocket.h"


using namespace std;
using namespace mongo;
using namespace socketlibrary;

struct pipe {
    int pipe;
};

class SendUpdate
{
    public:
        static void *spawn(void*  params_in);
    protected:
    private:
};

struct s_sut_params_in {
    const char* pipe_r;
    const char* pipe_w;
};

struct s_SUTMessage
{
    vector<Node<sockaddr_in>*> SocketList;
    BSONObj message;
};

#endif // SENDUPDATE_H
