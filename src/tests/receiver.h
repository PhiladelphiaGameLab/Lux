#ifndef RECEIVER_H
#define RECEIVER_H

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_BUF 1024
class receiver
{
public:
static void *receive(void *threadid);
};

void *receiver::receive(void *threadid)
{
long tid;
tid = (long)threadid;
printf("Hello World! It's me, thread #%ld!\n", tid);

int fd;
const char* myfifo ="./fifo_reader";
char buf[MAX_BUF];

//mkfifo(myfifo,0666);
fd=open(myfifo,O_RDONLY);
read(fd, buf, MAX_BUF);
printf(" Recieved: %s\n" , buf);
close(fd);

//return 0;

}

#endif

