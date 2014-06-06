#ifndef DBWRITER_H
#define DBWRITER_H


class DBWriter
{
    public:
        static void spawn(struct s_dbWriter params_in);
    protected:
    private:
};

typedef struct s_dbWriter
{
    char* pipe_r;
};

#endif // DBWRITER_H
