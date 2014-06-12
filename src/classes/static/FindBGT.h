#ifndef FINDBGT_H
#define FINDBGT_H
#define DATABASE_NAME "db.gameplay"


#include "HMBL.h"
#include <string>
#include <cstdlib>
#include <iostream>

#include "Authenticate.h"
#include "MD5.h"
#include "mongo/client/dbclient.h"
#include "mongo/bson/bson.h"
#include "mongo/db/json.h"

using namespace mongo;
using namespace std;


struct s_BGTInfo{
    int bgt_id;
    std::vector<int> buckets;
};


class FindBGT{
    public:
        static s_BGTInfo find(BSONObj clientDocument);
        static std::vector<int> findSurroundings(BSONObj clientDocument, int bgt_id);
    protected:
    private:
};

#endif // FINDBGT_H
