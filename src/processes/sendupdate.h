#ifndef SENDUPDATE_H
#define SENDUPDATE_H

typedef struct pipe {
    int pipe;
};

// typedef struct sendUpdates{
//     SocketList
//     BSONObj message;
// };

class SendUpdate
{
    public:
        static void SendUpdate::spawn(struct s_sut_params_in* params_in)
    protected:
    private:
};

struct s_sut_params_in {
    char* pipe_r;
    char* pipe_w;
};

#endif // SENDUPDATE_H
