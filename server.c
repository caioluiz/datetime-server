#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define LISTENQ 1024

int main(int argc, char* argv[]) {
    struct sockaddr_in servaddr;
    int port = 3500;
    
    // The user may use the default port
    // or input a different one from the command line.
    if (argc == 2) {
        port = atoi(argv[1]);
    } 
    printf("Port: %d\n", port);
    
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        printf("There was an error creating the socket: %s\n", strerror(errno));
        exit(1);
    }
    else printf("Socket created successfully, descriptor: %d\n", listenfd);
        
    bzero(&servaddr, sizeof servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    int resp_bind = bind(listenfd, (struct sockaddr*) &servaddr, sizeof servaddr);
    if (resp_bind == -1) {
        printf("There was an error binding the socket: %s\n", strerror(errno));
        exit(1);
    }
    else puts("Socket binded successfully");
        
    int resp_listen = listen(listenfd, LISTENQ);
    if (resp_listen == -1) {
        printf("There was an error listening to the socket: %s\n", strerror(errno));
        exit(1);
    }
    else printf("Listening on the socket %d\n", port);
             
    while (true) {
        int connfd = accept(listenfd, NULL, NULL);
        if (connfd == -1) {
            printf("There was an error accepting connection: %s\n", strerror(errno));
            exit(1);
        }
        else printf("Accepted socket %d\n", port);
        
        time_t ticks = time(NULL);  
        char buff[4096];   
        snprintf(buff, sizeof buff, "%.24s\r\n", ctime(&ticks));
        
        int written_bytes = write(connfd, buff, strlen(buff));
        if (written_bytes == -1) {
            printf("There was an error writing to the socket: %s\n", strerror(errno));
            exit(1);
        }
        if (written_bytes != strlen(buff)) {
            puts("The buffer was not entirely written to the socket");
            exit(1);
        }
        else puts("Sucessefully written to the socket");
        
        int resp_close = close(connfd);
        if (resp_close == -1) {
            printf("There was an error closing to the socket: %s\n", strerror(errno));
            exit(1);
        }
        else printf("Closed socket %d\n", port);
    }
        
    return 0;      
} 
