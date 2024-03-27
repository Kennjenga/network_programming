#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        error("Socket creation failed");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0) {
        error("Socket bind failed");
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) != 0) {
        error("Listen failed");
    }

    printf("Server listening...\n");

    while (1) {
        int len = sizeof(cli);
        connfd = accept(sockfd, (SA *)&cli, &len);
        if (connfd < 0) {
            error("Server accept failed");
        }

        // Read integers from client
        char buffer[MAX];
        bzero(buffer, MAX);
        read(connfd, buffer, sizeof(buffer));

        int num1, num2;
        sscanf(buffer, "%d %d", &num1, &num2);

        // Calculate sum
        int sum = num1 + num2;
        snprintf(buffer, MAX, "%d", sum);

        // Send sum back to client
        write(connfd, buffer, sizeof(buffer));

        close(connfd);
    }

    close(sockfd);
    return 0;
}