#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/errno.h>

#define SERV_PORT 9877
#define MAXLINE 4096
#define SA struct sockaddr

void sum(int sockfd);

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2) {
        printf("Usage: %s <Server IP>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        sum(sockfd);
        printf("Do you want to calculate another sum? (y/n): ");
        char choice;
        scanf(" %c", &choice);
        if (choice != 'y' && choice != 'Y') {
            break;
        }
    }

    close(sockfd);
    return 0;
}

void sum(int sockfd) {
    int num1, num2;

    printf("Enter two integers: ");
    scanf("%d %d", &num1, &num2);

    // Send the integers to the server
    write(sockfd, &num1, sizeof(num1));
    write(sockfd, &num2, sizeof(num2));

    // Receive the sum from the server
    int sum;
    read(sockfd, &sum, sizeof(sum));
    printf("Sum received from server: %d\n", sum);
}


