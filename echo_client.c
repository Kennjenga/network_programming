#include <sys/types.h> /* basic system data types */
#include <sys/socket.h> /* basic socket definitions */
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h> /* inet(3) functions */
#include <sys/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define SERV_PORT 9877
#define MAXLINE 4096
#define SA struct sockaddr 
void str_cli(FILE *fp, int sockfd);
int main(int argc, char **argv)
{
int sockfd;
struct sockaddr_in servaddr;
if (argc != 2)
{
printf("IP address not provided");
 }
sockfd = socket(AF_INET, SOCK_STREAM, 0);
bzero(&servaddr, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_port = htons(SERV_PORT);
inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
connect(sockfd,(SA *) &servaddr, sizeof(servaddr));
str_cli(stdin, sockfd); /* do it all */
exit(0);
}
void str_cli(FILE *fp, int sockfd)
{
char sendline[MAXLINE], recvline[MAXLINE];
while (fgets(sendline, MAXLINE, fp) != NULL) {
write(sockfd, sendline, strlen (sendline));
if (read(sockfd, recvline, MAXLINE) == 0)
printf("str_cli: server terminated prematurely");
fputs(recvline, stdout);
}
}
