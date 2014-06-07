#ifndef DBWRITER_H
#define DBWRITER_H


class DBWriter
{
    public:
        static void spawn(struct s_dbWriter_params_in params_in);
    protected:
    private:
};

typedef struct s_dbWriter_params_in
{
    char* pipe_r;
};

#endif // DBWRITER_H
