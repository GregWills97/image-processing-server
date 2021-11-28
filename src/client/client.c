#include <stdio.h>
#include <sys/stat.h>

#include "csapp.h"

void send_file(int fd, int port, char* filename, int filesize);
void make_request(int fd, int port, char* filename, int filesize);
void read_response(rio_t* rp);
void get_filetype(char* filename, char* filetype);

int main(int argc, char* argv[]) {
    int clientfd, port;
    char* file_name;
    socklen_t server_len;
    struct sockaddr_in server_addr;
    struct stat sbuf;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s, <port>\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);
    file_name = argv[2];

    if (stat(file_name, &sbuf) < 0) {
        fprintf(stderr, "Couldn't find file %s", argv[2]);
        exit(1);
    }

    clientfd = Open_clientfd("localhost", port);

    send_file(clientfd, port, file_name, sbuf.st_size);

    Close(clientfd);

    return 0;
}

void send_file(int fd, int port, char* filename, int filesize) {
    rio_t rio;

    Rio_readinitb(&rio, fd);
    make_request(fd, port, filename, filesize);
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

void make_request(int fd, int port, char* filename, int filesize) {
    int srcfd;
    char* srcfp;
    char filetype[MAXLINE], buf[MAXLINE];

    get_filetype(filename, filetype);
    sprintf(buf, "POST /grayscale HTTP/1.0\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Host: localhost:%d\r\n", port);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-Type: %s\r\n", filetype);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-Length: %d\r\n\r\n", filesize);
    Rio_writen(fd, buf, strlen(buf));

    srcfd = Open(filename, O_RDONLY, 0);
    srcfp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    Rio_writen(fd, srcfp, filesize);
    Munmap(srcfp, filesize);
}

void get_filetype(char* filename, char* filetype) {
    if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else
        strcpy(filetype, "text/plain");
}
