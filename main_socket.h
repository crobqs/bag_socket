#ifndef MAIN_SOCKET_H
#define MAIN_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void server_chat(int connfd) {
    char buf[MAX];
    int n;
    while (1) {
        bzero(buf, MAX);
        read(connfd, buf, sizeof buf);
        printf("From client: %s\t To client: ", buf);
        bzero(buf, MAX);
        n = 0;
        while ((buf[n++] = getchar()) != '\n');
        write(connfd, buf, sizeof buf);
        if (strncmp("exit", buf, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

void server_main() {
    int sockfd, connfd, len;
    struct sockaddr_in serve_addr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else {
        printf("socket successfully created...\n");
    }
    bzero(&serve_addr, sizeof serve_addr);
    serve_addr.sin_family = AF_INET;
    serve_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serve_addr.sin_port = htons(PORT);
    if (bind(sockfd, (SA*)&serve_addr, sizeof serve_addr)) {
        printf("socket bind failed...\n");
        exit(0);
    } else {
        printf("socket successfully binded...\n");
    }
    if (listen(sockfd, 5)) {
        printf("listen failed...\n");
        exit(0);
    } else {
        printf("server listening...\n");
    }
    len = sizeof cli;
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accrpt failed...\n");
        exit(0);
    } else {
        printf("server accept the client...\n");
    }
    server_chat(connfd);
    close(sockfd);
}

void client_chat(int sockfd) {
    char buf[MAX];
    int n;
    while (1) {
        bzero(buf, sizeof buf);
        printf("Enter the string: ");
        n = 0;
        while ( (buf[n++] = getchar()) != '\n' );
        write(sockfd, buf, sizeof buf);
        bzero(buf, sizeof buf);
        read(sockfd, buf, sizeof buf);
        printf("From Server: %s", buf);
        if (strncmp(buf, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

void client_main() {
    int sockfd, connfd;
    struct sockaddr_in serve_addr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else {
        printf("socket successfully created...\n");
    }
    bzero(&serve_addr, sizeof serve_addr);
    serve_addr.sin_family = AF_INET;
    serve_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serve_addr.sin_port = htons(PORT);
    if (connect(sockfd, (SA*)&serve_addr, sizeof serve_addr)) {
        printf("connection with the server failed...\n");
        exit(0);
    } else {
        printf("connected to the server...\n");
    }
    client_chat(sockfd);
    close(sockfd);
}


void main_fake() {
    char mode[32];
    memset(mode, 0, sizeof mode);
    printf("mode: ");
    scanf("%s", mode);
    if ( !strcmp(mode, "server") ) {
        server_main();
    }
    else if ( !strcmp(mode, "client") ) {
        client_main();
    } else { ; }
}

#endif /* MAIN_SOCKET_H */