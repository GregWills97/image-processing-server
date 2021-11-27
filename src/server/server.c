#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "image_process.hpp"
#include "csapp.h"

void serve_request(int fd);
void format_success(int fd, char* shortmsg, char* longmsg);
void format_error(int fd, char* cause, char* error_num, char* shortmsg, char* longmsg);
void read_request_headers(rio_t* rp, char* content_type, char* content_size);
void get_filetype(char* filename, char* filetype);

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
    char image_type[MAXLINE], image_size[MAXLINE];
    struct stat sbuf;
    rio_t rio;

    Rio_readinitb(&rio, fd);
    Rio_readlineb(&rio, buf, MAXLINE);
    sscanf(buf, "%s %s %s", method, uri, version);
    read_request_headers(&rio, image_type, image_size);

    //GET request just return sample image
    if (!strcasecmp(method, "GET")) {
        if (!strcmp(uri, "/grayscale")) {
            grayscale_file("./test_images/test1.jpg", "./output/gray_image1.jpg");
        }
        else if (!strcmp(uri, "/edge-detect")) {
            edge_detect_file("./test_images/test1.jpg", "./output/edge_detect_image1.jpg", 25, 50, 3);
        }
        else if (!strcmp(uri, "/blur")) {
            blur_file("./test_images/test1.jpg", "./output/blur_image1.jpg", 20);
        }
        else {
            format_error(fd, uri, "404", "Missing", "Invalid type of processing method");
            return;
        }

        format_success(fd, "Image processed", 
                "Image was succuessfully processed and should be lcoated in output directory");
        return;
    }

    //POST request should be file upload
    if (!strcasecmp(method, "POST")) {
        //read and print out request headers
        format_error(fd, method, "501", "Not Implemented",
            "Simple image server does not implement this method");
        return;
    }

    format_error(fd, method, "501", "Not Implemented",
        "Simple image server does not implement this method");
}

void read_request_headers(rio_t *rp, char* content_type, char* content_size) {
    char buf[MAXLINE];
    char* ptr;

    //print request information while we read
    while (strcmp(buf, "\r\n")) {
        Rio_readlineb(rp, buf, MAXLINE);
        if (strstr(buf, "Content-Type:")) {
            ptr = index(buf, ' ');
            if (ptr)
                strcpy(content_type, ptr+1);
        }
        else if (strstr(buf, "Content-Length:")) {
            ptr = index(buf, ' ');
            if (ptr)
                strcpy(content_size, ptr+1);
        }
        printf("%s", buf);
    }
}

void format_success(int fd, char* shortmsg, char* longmsg) {
    char buf[MAXLINE], body[MAXLINE];

    sprintf(body, "<html><title>Simple Image Sucess</title>");
    sprintf(body, "%s<body bgcolor=\"ffffff\">\r\n", body);
    sprintf(body, "%sSUCCESS: %s\r\n", body, shortmsg);
    sprintf(body, "%s<p>%s\r\n", body, longmsg);
    sprintf(body, "%s<hr><em>The Simple Image Server</em>\r\n", body);

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Simple Image Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: ^d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}

void format_error(int fd, char* cause, char* error_num, char* shortmsg, char* longmsg) {
    char buf[MAXLINE], body[MAXLINE];

    sprintf(body, "<html><title>Simple Image Server Error</title>");
    sprintf(body, "%s<body bgcolor=\"ffffff\">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, error_num, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Simple Image Server</em>\r\n", body);


    sprintf(buf, "HTTP/1.0 %s %s\r\n", error_num, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Simple Image Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Connection: Closed\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}
