#ifndef INITIALIZE_H
#define INITIALIZE_H
#define DATABASE_NAME "Gameplay_DB"

#include "Authenticate.h"
#include "CGI.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"
#include "mongo/db/json.h"
#include "FindBGT.h"

using namespace mongo;
using namespace std;


// Stand Alone CGI script

class Initialize{

    public:
        static BSONObj getClientDoc(string EUID);
        static void relevantDocuments(s_BGTInfo bgtInfo);
    protected:
    private:

};

#endif // INITIALIZE_H
