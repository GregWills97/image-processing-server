#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include "image_process.hpp"
#include "csapp.h"
#include "producer_consumer_utils.h"

void serve_request(int fd);
void format_success(int fd, char* shortmsg, char* longmsg);
void format_error(int fd, char* cause, char* error_num, char* shortmsg, char* longmsg);
void read_request_headers(rio_t* rp, char* content_type, char* content_size);
void read_image_file(rio_t* rp, int filesize, char* filename);
void send_image_file(int fd, char* filename);
void get_filetype(char* filename, char* filetype);
void add_request(int fd, queue *fifo);

int main (int argc, char* argv[]) {
    int listenfd, connfd;
    int port;
    socklen_t client_len;
    struct sockaddr_in client_addr;

    queue *client_requests_fifo;
    pthread_t produce, consume;
    client_requests_fifo = queueInit();

    if (client_requests_fifo == NULL){
      printf("%s", "Client Request Queue Init Failed.\n");
      exit(1);
    }
   
    if (argc != 2) {
        fprintf(stderr, "Usage: %s, <port>\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);

    listenfd = Open_listenfd(port);

    while (1) {
        client_len = sizeof(client_addr);
	// Add connection configuration to queue for worker threads.
	
        connfd = Accept(listenfd, (SA *)&client_addr, &client_len);
	add_request(confd, client_requests_fifo);
	
	// serve_request(connfd);
        //Close(connfd);
    }

    return 0;
}


void add_request(int fd, queue *fifo){

  printf("%s","Connection Request Receieved. Attempting to Add to  Queue..");
  pthread_mutex_lock (fifo->mut);
  while(fifo->full){
    pthread_cond_wait(fifo->notFull, fifo->mut);
  }

  queueAdd(fifo, fd);
  pthread_mutex_unlock(fifo->mut);
  pthread_cond_signal(fifo->notEmpty);
  printf("%s", "Connection Request Successfully Added to Queue.");
 
}
  
  
 
  
//TODO: Convert to Worker Thread
void serve_request(int fd) {
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char image_type[MAXLINE], image_size[MAXLINE];
    char image_file[MAXLINE], output_file[MAXLINE];
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
                "Image was succuessfully processed and should be located in output directory");
        return;
    }

    //POST request file upload
    if (!strcasecmp(method, "POST")) {
        if (!strcmp(image_type, "image/jpeg\r\n")) {
            read_image_file(&rio, atoi(image_size), image_file);
            strncpy(output_file, image_file, (strlen(image_file)-4)); //remove extension
            if (!strcmp(uri, "/grayscale")) {
                strcat(output_file, "-grayscale.jpg");
                grayscale_file(image_file, output_file);
            }
            else if (!strcmp(uri, "/edge-detect")) {
                strcat(output_file, "-edge-detect.jpg");
                edge_detect_file(image_file, output_file, 25, 50, 3);
            }
            else if (!strcmp(uri, "/blur")) {
                strcat(output_file, "-blur.jpg");
                blur_file(image_file, output_file, 20);
            }
            else {
                format_error(fd, uri, "404", "Missing", "Invalid type of processing method");
                return;
            }
            send_image_file(fd, output_file);
        }
        else {
            format_error(fd, method, "404", "File type not supported",
                "Simple image server cannot support this file type");
        }
        return;
    }

    format_error(fd, method, "501", "Not Implemented",
        "Simple image server does not implement this method");
}

void read_request_headers(rio_t* rp, char* content_type, char* content_size) {
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

void read_image_file(rio_t* rp, int filesize, char* filename) {
    char buf[MAXBUF];
    char* ptr;
    int srcfd;

    srand(time(0));
    sprintf(filename, "./downloads/%d.jpg", rand());

    srcfd = Open(filename, O_CREAT | O_RDWR, 0664);
    Rio_readnb(rp, buf, filesize);
    Write(srcfd, buf, filesize);
    Close(srcfd);
}

void send_image_file(int fd, char* filename) {
    char buf[MAXLINE];
    int srcfd;
    char* srcfp;
    struct stat sbuf;

    stat(filename, &sbuf);

    srcfd = Open(filename, O_RDONLY, 0);
    srcfp = Mmap(0, sbuf.st_size, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Simple Image Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-Type: image/jpeg\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-Length: %d\r\n\r\n", (int)sbuf.st_size);
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, srcfp, sbuf.st_size);
    Munmap(srcfp, sbuf.st_size);
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
    sprintf(buf, "Content-Type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-Length: %d\r\n\r\n", (int)strlen(body));
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
