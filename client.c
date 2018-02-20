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

int main(int argc, char* argv[]) {
    struct sockaddr_in servaddr;
    uint32_t buffer;
    char ip_address[36];
    int port;
    
    // The user may use the local default address
    // or input a different one from the command line.
    if (argc == 3) {
        strcpy(ip_address, argv[1]);
        port = atoi(argv[2]);
    } 
    else {
        strcpy(ip_address, "127.0.0.1");
        port = 3500;
    }
    printf("IP: %s, port: %d\n", ip_address, port);
    
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        printf("There was an error creating the socket: %s\n", strerror(errno));
        exit(1);
    }
    else printf ("Socket created successfully, descriptor: %d\n", socketfd);
    
    bzero(&servaddr, sizeof servaddr);    
    int resp_inet_pton = inet_pton(AF_INET, ip_address, &servaddr.sin_addr);
    switch (resp_inet_pton) {
        case 1:
            puts("The address was converted successfully");
            break;
        case 0:
            puts("*Src doesn't not contain a valid ip address");
            exit(1);
        case -1:
            puts("The address family is not valid");
            exit(1);
        default:
            puts("Unsuccessful call to inet_pton");
            exit(1);
    }
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    
    int connect_resp = connect(socketfd, (struct sockaddr*) &servaddr, sizeof servaddr);
    switch (connect_resp) {
        case 0:
            puts("Connected successfully");
            break;
        case -1:
            printf("There was an error connecting: %s\n", strerror(errno));
            exit(1);
    }
    

    char daytime_buffer[64];
    int read_bytes = read(socketfd, daytime_buffer, sizeof daytime_buffer);
    daytime_buffer[read_bytes] = '\0';
    fputs(daytime_buffer, stdout);
    
    return 0;
} 
