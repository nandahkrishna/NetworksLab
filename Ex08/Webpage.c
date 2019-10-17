#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<netinet/tcp.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netdb.h>
int socket_connect(char *host, in_port_t port)
{
    struct hostent *hp;
    struct sockaddr_in addr;
    int on = 1, sock;
    if((hp = gethostbyname(host)) == NULL) {
        printf("gethostbyname error!\n");
        exit(0);
    }
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
    if(sock == -1){
        printf("setsockopt error!\n");
        exit(0);
    }
    if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1){
        printf("connect error!\n");
        exit(0);
    }
    return sock;
}
int main(int argc, char *argv[])
{
    int fd;
    char buffer[1024];
    if(argc < 3){
        printf("Usage: %s <hostname> <port>\n", argv[0]);
        exit(0);
    }
    fd = socket_connect(argv[1], atoi(argv[2]));
    write(fd, "GET / \r\n\r\n", strlen("GET /\r\n\r\n"));
    bzero(buffer, 1024);
    while(read(fd, buffer, 1023) != 0) {
        printf("%s", buffer);
        bzero(buffer, 1024);
    }
    shutdown(fd, SHUT_RDWR);
    close(fd);
    return 0;
}
