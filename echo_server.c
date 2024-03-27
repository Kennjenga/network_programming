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
#define LISTENQ 5
#define SA struct sockaddr 
void str_echo(int sockfd);
int main(int argc, char **argv)
{
int listenfd, connfd;
pid_t childpid;
socklen_t clilen;
struct sockaddr_in cliaddr, servaddr;
listenfd = socket (AF_INET, SOCK_STREAM, 0);
bzero(&servaddr, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
servaddr.sin_port = htons (SERV_PORT);
bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
listen(listenfd, LISTENQ);
for ( ; ; ) {
clilen = sizeof(cliaddr);
connfd = accept(listenfd, (SA *) &cliaddr, &clilen);
if (connfd>0){
printf("received a connection from: %s port %d\n",
inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
}
if ( (childpid = fork()) == 0) { /* child process */
close(listenfd); /* close listening socket */
str_echo(connfd); /* process the request */
exit (0);
}
close(connfd); /* parent closes connected socket */
}
}
/*str_echo function reads the lines from the client and echo them back to 
the client */
void str_echo(int sockfd)
{
ssize_t n; /* Read "n" bytes from a descriptor. */
char line[MAXLINE];
for(;;){
if ((n=read(sockfd, line, MAXLINE))== 0)
return; /*connection closed by other end*/
write(sockfd, line, n);
}
}
