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
	int len;
	int sockfd, newfd, n;
	struct sockaddr_in servaddr, cliaddr;
	char buff[1024];
	char str[1000];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		perror("Cannot create socket!\n");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(7228);
	if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		perror("Bind error!\n");
	listen(sockfd,2);
	len = sizeof(cliaddr);
	newfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);
	n = read(newfd, buff, sizeof(buff));
	printf("File name: %s\n", buff);
	int fd = open(buff, O_RDONLY);
	if(fd < 0) {
		strcpy(buff, "Does not exist!\n");
	}
	else {
		n = 0;
		bzero(buff, sizeof(buff));
		read(fd, buff, 1024);
		close(fd);
	}
	n = write(newfd, buff, sizeof(buff));
	close(newfd);
	close(sockfd);
	return 0;
}
