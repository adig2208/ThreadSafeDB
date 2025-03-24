#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "proj2.h"
#include "database.h"

#define PORT 5000

void handle_work(int sock_fd);
void listener(int port);

void listener(int port) {
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    if(sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    struct sockaddr_in addr = {.sin_family = AF_INET, .sin_port = htons(port), .sin_addr.s_addr = 0};
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("can't bind");
        exit(1);
    }
    if (listen(sock,2) < 0){
        perror("listen failed");
        exit(1);
    }
    printf("Server listening on port %d\n",port);
    while(1){
        int fd = accept(sock,NULL,NULL);
        if (fd < 0) {
            perror("Accept failed");
            continue;
        }
        printf("Connection Established.. \n");
        handle_work(fd);
        close(fd);
    }

}

void handle_work(int sock_fd) {
    struct request req;
    struct request response;
    char buf_write[4096];
    char buf_read[4096];
    int len;
    int status;

    if (read(sock_fd, &req, sizeof(req)) <=0) {
        perror("Failed to read request");
        return;
    }

    switch (req.op_status) {
        case 'W':
            len = atoi(req.len);
            if (len > 4096) {
                len = 4096;
            }
            if (read(sock_fd, buf_write, len) != len) {
                perror("Failed to read provided data");
                response.op_status = 'X';
                break;
            }
            status = db_write(req.name, buf_write, len);
            if (status == 0) {
                response.op_status = 'K';
            } else {
                response.op_status = 'X';
            }
            snprintf(response.len, sizeof(response.len), "%d", 0);
            break;
        case 'R':
            len = db_read(req.name, buf_read);
            if (len > 0) {
                response.op_status = 'K';
                snprintf(response.len, sizeof(response.len), "%.7d", len);
            } else {
                response.op_status = 'X';
                snprintf(response.len, sizeof(response.len), "%d", 0);
            }
            break;
        case 'D':
            status = db_delete(req.name);
            if (status == 0) {
                response.op_status = 'K';
                snprintf(response.len, sizeof(response.len), "%d", 0);
                break;
            } 
            response.op_status = 'X';
            snprintf(response.len, sizeof(response.len), "%d", 0);
            break;
        default:
            perror("invalid operation");
            response.op_status = 'X';
            snprintf(response.len, sizeof(response.len), "%d", 0);
            break;
    }

    memset(response.name, 0 , sizeof(response.name));
    write(sock_fd, &response, sizeof(response));
    if (req.op_status == 'R' && response.op_status == 'K') {
        write(sock_fd, buf_read, len);
    }
}

int main() {
    listener(PORT);
    return 0;
}