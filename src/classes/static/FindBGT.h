#ifndef FINDBGT_H
#define FINDBGT_H


/*
// Initialize.h
typedef struct s_BGTInfo{
    int bgt_id;
    std::list<int> buckets;
}


*/


class FindBGT{
    public:
        s_BGTInfo find(BSONObj clientDocument);
    protected:
    private:
};

#endif // FINDBGT_H
