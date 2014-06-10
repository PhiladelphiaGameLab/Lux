#ifndef SENDUPDATE_H
#define SENDUPDATE_H

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include <string>
#include <cstdlib>
#include <iostream>

#include <netinet/in.h>

#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"
#include "mongo/db/json.h"

#include "HMBL.h"

using namespace std;
using namespace mongo;


struct pipe {
    int pipe;
};

// typedef struct sendUpdates{
//     SocketList
//     BSONObj message;
// };

class SendUpdate
{
    public:
        static void spawn(struct s_sut_params_in* params_in);
    protected:
    private:
};

struct s_sut_params_in {
    char* pipe_r;
    char* pipe_w;
};

typedef struct s_SUTMessage
{
    BSONObj message;
    std::vector<Node *> SocketList;
}

#endif // SENDUPDATE_H
