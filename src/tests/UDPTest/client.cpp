/*
 * client.cpp - A simple UDP client.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFSIZE 1024

/* 
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

int main() {
    int sockfd;
    int n;
    socklen_t serverlen;
    unsigned short portno = 3000;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char *hostname = "localhost";
    char buf[BUFSIZE];
    
    /* socket: create the socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
	error("ERROR opening socket");
    }
    
    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }
    
     /* build the server's Internet address */
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(portno);
    while (1) {
    /* get a message from the user */
    bzero(buf, BUFSIZE);
    printf("Please enter msg: ");
    fgets(buf, BUFSIZE, stdin);
    
    /* send the message to the server */
    serverlen = sizeof(server_addr);
    n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&server_addr, 
	       serverlen);
    if (n < 0)  {
	error("ERROR in sendto");
    }
    
    
    /* print the server's reply */
    n = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *)&server_addr, &serverlen);
    if (n < 0) 
      error("ERROR in recvfrom");
    printf("Echo from server: %s\n", buf);
    }
    return 0;
    
}
