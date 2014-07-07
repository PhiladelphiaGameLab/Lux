#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_BUF 1024

int main()
{

int fd;
char* myfifo ="./fifo_reader";
char buf[MAX_BUF];

//mkfifo(myfifo,0666);

fd=open(myfifo,O_RDONLY);
read(fd, buf, MAX_BUF);
printf(" Recieved: %s\n" , buf);
close(fd);

return 0;

}

