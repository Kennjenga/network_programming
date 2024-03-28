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

int main() {
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    listen(listenfd, 5);

    printf("Server listening...\n");

    while (1) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (SA *)&cliaddr, &clilen);
        if (connfd < 0) {
            perror("Server accept failed");
            continue;
        }

        if ((childpid = fork()) == 0) { /* child process */
            close(listenfd); /* close listening socket */
            while (1) {
                sum(connfd);
            }
            exit(0);
        }

        close(connfd); /* parent closes connected socket */
    }

    close(listenfd);
    return 0;
}

void sum(int sockfd) {
    int num1, num2;

    printf("Waiting for two integers from the client...\n");
    read(sockfd, &num1, sizeof(num1));
    read(sockfd, &num2, sizeof(num2));

    // Calculate the sum
    int sum = num1 + num2;

    // Send the sum back to the client
    write(sockfd, &sum, sizeof(sum));
}
