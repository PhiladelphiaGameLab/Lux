#ifndef SENDUPDATE_H
#define SENDUPDATE_H
typedef struct pipe{
    int pipe;
};

typedef struct sendUpdates{
    SocketList
    BSONObj message;
};

class SendUpdate
{
    public:
        static void SendUpdate::spawn(struct sendUpdateArgs params_in)
    protected:
    private:
};

#endif // SENDUPDATE_H
