#ifndef INITIALIZE_H
#define INITIALIZE_H

#include "findBGT.h"

#DEFINE DATABASE_NAME "dev.objects"

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
