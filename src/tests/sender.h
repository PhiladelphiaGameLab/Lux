#ifndef SENDER_H
#define SENDER_H

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>

class sender
{
public:
static void *send(void *threadid);
};

void *sender::send(void *threadid)
{

long tid;
tid = (long)threadid;
printf("Hello World! It's me, thread #%ld!\n", tid);

int fd;
const char* myfifo ="./fifo_reader";

mkfifo(myfifo,0666);

fd=open(myfifo,O_WRONLY);
write(fd, "Hi", sizeof("Hi"));
//close(fd);
//
////unlink(myfifo);
//
//return 0;
//
}
#endif
