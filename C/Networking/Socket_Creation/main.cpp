/// LINUX

#include <stdio.h>                 //H
#include <stdlib.h>                //E
#include <string.h>                //A
#include <unistd.h>                //D
#include <sys/types.h>             //E
#include <sys/socket.h>            //R
#include <netinet/in.h>            //FILE
#include <netdb.h>                 //S

#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(char *err) {
    perror(err);
    exit(0);
}
              struct student
              {
                  char name[16];
                  unsigned short student_id;
                  char host[16];
                  unsigned short pid;
                  char email[32];

              }r1;

void main(int argc, char **argv) {
    int sockfd, portno, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];
struct student r1 = {"Parikshit Tiwari", htons(0x6160), "Parikx-Laptop", htons(getpid()),"parikshit@gmail.com"};

    if (argc != 3) {
       fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }
    hostname = argv[1];
    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);//socket created
    if (sockfd < 0)
        error("ERROR opening socket");

    /* gethostbyname: get the server's DNS entry */
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, hostname %s\n does not exist", hostname);
        exit(0);
    }

    /* build the server's Internet address */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);


   /* send the message to the server */
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, (void *) &r1, sizeof(r1), 0, (struct sockaddr *)&serveraddr, serverlen);
    printf("%d", n);//Test to check the size of data send to the server
    if (n < 0)
      error("ERROR in sendto");

    close(sockfd);
}
