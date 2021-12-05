#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/stat.h>

#include "image_process.hpp"
#include "ips_queue.h"
#include "csapp.h"

#define NUM_THREADS 3

void *listen_for_request(void* args);
void *serve_requests(void* args);
void process_request (int fd);
void format_success(int fd, char* shortmsg, char* longmsg);
void format_error(int fd, char* cause, char* error_num, char* shortmsg, char* longmsg);
void read_request_headers(rio_t* rp, char* content_type, char* content_size);
void read_image_file(rio_t* rp, int filesize, char* filename);
void send_image_file(int fd, char* filename);
void get_filetype(char* filename, char* filetype);

struct thread_args {
    int port_num;
    queue* process_queue;
};

int main (int argc, char* argv[]) {
    struct thread_args arguments;
    pthread_t listen_thread;
    pthread_t worker_threads[NUM_THREADS];
    int port;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s, <port>\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);

    arguments.port_num = port;
    arguments.process_queue = queue_init();

    //Create worker threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&worker_threads[i], NULL, serve_requests, &arguments);
    }
    //Create main thread
    pthread_create(&listen_thread, NULL, listen_for_request, &arguments);

    //Cleanup
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(worker_threads[i], NULL);
    }
    pthread_join(listen_thread, NULL);
    queue_delete(arguments.process_queue);
    return 0;
}

void *listen_for_request(void* args) {
    int listenfd;
    struct thread_args arguments;
    int port;
    queue* process_queue;

    /* Dereference arguments */
    arguments = *(struct thread_args *)args;
    port = arguments.port_num;
    process_queue = arguments.process_queue;

    /* Start listening */
    listenfd = Open_listenfd(port);
    while (1) {
        int connfd;
        struct sockaddr_in client_addr;
        socklen_t client_len;

        client_len = sizeof(client_addr);
        connfd = Accept(listenfd, (SA *)&client_addr, &client_len);
        
        /* Add to queue */
        pthread_mutex_lock(process_queue->mut);
        while(process_queue->full) {
            pthread_cond_wait(process_queue->not_full, process_queue->mut);
            printf("Queue is full!\n");
        }
        queue_add(process_queue, connfd);
        pthread_mutex_unlock(process_queue->mut);
        pthread_cond_signal(process_queue->not_empty);
    }
    return NULL;
}

void *serve_requests(void* args) {
    int fd;
    struct thread_args arguments;
    queue* process_queue;

    /* Dereference arguments */
    arguments = *(struct thread_args *)args;
    process_queue = arguments.process_queue;

    /* Keep checking queue */
    while(1) {
        pthread_mutex_lock(process_queue->mut);
        while(process_queue->empty) {
            pthread_cond_wait(process_queue->not_empty, process_queue->mut);
        }
        queue_del(process_queue, &fd);
        pthread_mutex_unlock(process_queue->mut);
        pthread_cond_signal(process_queue->not_full);

        /* Process request */
        process_request(fd);
        sleep(10);
    }
    return NULL;
}

void process_request(int fd) {
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char image_type[MAXLINE], image_size[MAXLINE];
    char image_file[MAXLINE], output_file[MAXLINE];
    struct stat sbuf;
    rio_t rio;

    Rio_readinitb(&rio, fd);
    Rio_readlineb(&rio, buf, MAXLINE);
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);
    read_request_headers(&rio, image_type, image_size);

    //POST request file upload
    if (!strcasecmp(method, "POST")) {
        if (!strcmp(image_type, "image/jpeg\r\n")) {
            read_image_file(&rio, atoi(image_size), image_file);
            strncpy(output_file, image_file, (strlen(image_file)-4)); //remove extension
            if (!strcmp(uri, "/grayscale")) {
                strcat(output_file, "-grayscale.jpg");
                grayscale_file(image_file, output_file);
                send_image_file(fd, output_file);
            }
            else if (!strcmp(uri, "/edge-detect")) {
                strcat(output_file, "-edge-detect.jpg");
                edge_detect_file(image_file, output_file, 25, 50, 3);
                send_image_file(fd, output_file);
            }
            else if (!strcmp(uri, "/blur")) {
                strcat(output_file, "-blur.jpg");
                blur_file(image_file, output_file, 20);
                send_image_file(fd, output_file);
            }
            else {
                format_error(fd, uri, "404", "Missing", "Invalid type of processing method");
            }
        }
        else {
            format_error(fd, method, "404", "File type not supported",
                "Simple image server cannot support this file type");
        }
        Close(fd);
        return;
    }

    format_error(fd, method, "501", "Not Implemented",
        "Simple image server does not implement this method");
    Close(fd);
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
