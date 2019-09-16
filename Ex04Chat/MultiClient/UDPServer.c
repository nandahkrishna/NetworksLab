#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
int main(int argc, char **argv)
{
	unsigned int len;
	int sockfd, newfd, n = 0;
	struct sockaddr_in servaddr, cliaddr;
	char buff[1024];
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
		perror("Cannot create socket!\n");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(7228);
	if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		perror("Bind error!\n");
	len = sizeof(cliaddr);
	while(1) {
		n = recvfrom(sockfd, buff, sizeof(buff), MSG_WAITALL, (struct sockaddr*)&cliaddr, &len);
		if(n != 0) printf("%s\n", buff);
		strcpy(buff, "\0");
	}
	close(sockfd);
	return 0;
}
