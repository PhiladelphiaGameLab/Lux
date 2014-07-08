
#ifndef ID_TEST_H
#define ID_TEST_H
#define DATABASE_NAME "Luxdb.gameplay"

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

#include <time.h>

using namespace mongo;
using namespace std;
//using namespace socketlibrary;


// static class (thread)



class id_test{
    public:
        static void *spawn(void* params_in);
    protected:
    private:
};

#endif // BATTLEGROUND_H
