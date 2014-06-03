#ifndef FINDBGT_H
#define FINDBGT_H

class FindBGT{
    public:
        s_BGTInfo find(BSONObj clientDocument);
        std::vector<int> findSurroundings(BSONObj clientDocument, int bgt_id);
    protected:
    private:
};

#endif // FINDBGT_H
