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

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <integer1> <integer2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct sockaddr_in servaddr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        error("Socket creation failed");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
    servaddr.sin_port = htons(PORT);

    // Connect to server
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0) {
        error("Connection to server failed");
    }

    // Send integers to server
    char buffer[MAX];
    snprintf(buffer, MAX, "%s %s", argv[1], argv[2]);
    write(sockfd, buffer, sizeof(buffer));

    // Receive sum from server
    bzero(buffer, MAX);
    read(sockfd, buffer, sizeof(buffer));
    printf("Sum received from server: %s\n", buffer);

    close(sockfd);
    return 0;
}
