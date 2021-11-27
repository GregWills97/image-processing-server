#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "image_process.hpp"
#include "csapp.h"

void serve_request(int fd);
void format_error(int fd, char* cause, char* error_num, char* shortmsg, char* longmsg);
void read_request_headers(rio_t* rp);
void get_filetype(char* filename, char* filetype);
void parse_uri(char* uri, char* filename, char* args);

int main (int argc, char* argv[]) {
    int listenfd, connfd;
    int port;
    socklen_t client_len;
    struct sockaddr_in client_addr;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s, <port>\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);

    listenfd = Open_listenfd(port);

    while (1) {
        client_len = sizeof(client_addr);
        connfd = Accept(listenfd, (SA *)&client_addr, &client_len);
        serve_request(connfd);
        Close(connfd);
    }

    return 0;
}

void serve_request(int fd) {
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], args[MAXLINE];
    struct stat sbuf;
    rio_t rio;

    Rio_readinitb(&rio, fd);
    Rio_readlineb(&rio, buf, MAXLINE);
    sscanf(buf, "%s %s %s", method, uri, version);

    //read and print out request headers
    read_request_headers(&rio);

    //GET request just return sample image
    if (!strcasecmp(method, "GET")) {
        parse_uri(uri, filename, args);

        if (!strcmp(filename, "/grayscale")) {
            grayscale_file("./test_images/test1.jpg", "./output/gray_image1.jpg");
        }
        else if (!strcmp(filename, "/edge-detect")) {
            edge_detect_file("./test_images/test1.jpg", "./output/gray_image1.jpg", 25, 50, 3);
        }
        if (!strcmp(filename, "/blur")) {
            blur_file("./test_images/test1.jpg", "./output/gray_image1.jpg", 20);
        }
    }

    if (!strcasecmp(method, "POST")) {
        format_error(fd, method, "501", "Not Implemented",
        "Simple image server does not implement this method");
        return;
    }

}

void read_request_headers(rio_t *rp) {
    char buf[MAXLINE];

    //print request information while we read
    Rio_readlineb(rp, buf, MAXLINE);
    while (strcmp(buf, "\r\n")) {
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
}

void parse_uri(char* uri, char* filename, char* args) {
    char* ptr;
    strcpy(args, "");
    strcpy(filename, ".");
    strcpy(filename, uri);
}


void format_error(int fd, char* cause, char* error_num, char* shortmsg, char* longmsg) {
    char buf[MAXLINE];

    sprintf(buf, "HTTP/1.0 %s %s\r\n", error_num, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Simple Image Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Connection: Closed\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));
}
