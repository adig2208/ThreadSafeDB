#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "proj2.h"

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
    read(sock_fd, &req, sizeof(req));
    printf("Received op: %c, key: %s, len: %s\n", req.op_status, req.name, req.len);
}

int main() {
    listener(PORT);
    return 0;
}