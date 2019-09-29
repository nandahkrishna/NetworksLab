#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
int main(int argc, char **argv)
{
    unsigned int len;
    int sockfd, n = 0;
    struct sockaddr_in servaddr, cliaddr;
    char buff[100];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
        perror("Cannot create socket!\n");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(7228);
    len = sizeof(servaddr);
    while(1) {
        printf("Domain Name: ");
        scanf("%s", buff);
        n = sendto(sockfd, buff, sizeof(buff), MSG_DONTWAIT, (struct sockaddr*)&servaddr, sizeof(servaddr));
        n = recvfrom(sockfd, buff, sizeof(buff), MSG_WAITALL, (struct sockaddr*)&servaddr, &len);
        printf("The IP address is %s\n", buff);
    }
    close(sockfd);
    return 0;
}
