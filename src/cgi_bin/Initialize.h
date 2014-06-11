#ifndef INITIALIZE_H
#define INITIALIZE_H
#define DATABASE_NAME "dev.objects"

#include "Authenticate.h"
#include "CGI.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"
#include "mongo/db/json.h"

using namespace mongo;
using namespace std;

#include "findBGT.h"


// Stand Alone CGI script


typedef struct s_BGTInfo{
    int bgt_id;
    std::vector<int> buckets;
};


class Initialize{

    public:
        static BSONObj getClientDoc(EUID);
        static void relevantDocuments(BGTInfo bgtInfo){
    protected:
    private:

}

#endif // INITIALIZE_H
