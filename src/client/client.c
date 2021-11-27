#include <stdio.h>

#include "csapp.h"

void make_request(int fd, char* buf, int port);
void send_request(int fd, int port, char* filename);
void read_response(rio_t* rp);

int main(int argc, char* argv[]) {
    int clientfd;
    int port;
    socklen_t server_len;
    struct sockaddr_in server_addr;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s, <port>\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);

    clientfd = Open_clientfd("localhost", port);

    send_request(clientfd, port, "");

    Close(clientfd);

    return 0;
}

void send_request(int fd, int port, char* filename) {
    rio_t rio;
    char buf[MAXLINE];

    Rio_readinitb(&rio, fd);
    make_request(fd, buf, port);
    read_response(&rio);
}


void read_response(rio_t* rp) {

    char buf[MAXLINE];
    char* ptr;
    int count = 0;

    //print request information while we read
    while (strcmp(buf, "\r\n")) {
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
}

void make_request(int fd, char* buf, int port) {
    sprintf(buf, "POST /grayscale HTTP/1.0\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Host: localhost:%d\r\n", port);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-Type: image/jpeg\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-Length: 127\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));
}
