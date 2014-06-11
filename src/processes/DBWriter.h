#ifndef DBWRITER_H
#define DBWRITER_H
#define DATABASE_NAME "Gameplay_DB"


#include <string>
#include <cstdlib>
#include <iostream>

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "MD5.h"
#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"
#include "mongo/db/json.h"

using namespace mongo;
using namespace std;


class DBWriter
{
    public:
        static void *spawn(void* params_in);
    protected:
    private:
};

struct s_dbWriter_params_in{
    const char* pipe_r;
};

#endif // DBWRITER_H
