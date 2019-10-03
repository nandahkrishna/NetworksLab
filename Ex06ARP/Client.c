#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include<ctype.h>
int main(int argc, char *argv[])
{
    char *mac, *ip;
    mac = (char*)malloc(100);
    ip = (char*)malloc(100);
    for(int i = 0; i < 100; i++) ip[i] = '\0';
    FILE* fp = popen("ifconfig -a | grep -Po 'HWaddr \\K.*$'", "r");
    if(fgets(mac, 99, fp) != NULL) {
        printf("Client MAC Address is: %s", mac);
    }
    mac[17] = '\0';
    fp =  popen("ip route get 8.8.4.4 | head -1 | awk '{print $7}'", "r");
    if (fgets(ip, 99, fp) != NULL) {
        printf("Client IP Address is: %s",ip);
        ip[strlen(ip)-1] = '\0';
    }
    fclose(fp);
    int cfd = socket(AF_INET,SOCK_STREAM, 0);
    struct sockaddr_in serveraddr, clientaddr;
    bzero(&serveraddr, sizeof(struct sockaddr_in));
    bzero(&clientaddr, sizeof(struct sockaddr_in));
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(7891);
    connect(cfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    char *buff;
    char *buff1 = "null";
    buff = (char*)malloc(100);
    int n1 = 0;
    while(n1 == 0)
        n1 = read(cfd, buff, 100);
    buff[n1] = '\0';
    printf("Received IP request is %s\n", buff);
    if(strcmp(buff, ip) == 0)
    {
        printf("IP matched, sending MAC Address to server!\n");
        write(cfd, mac, strlen(mac));
        read(cfd, buff, 100);
        printf("Received message: %s\n",buff);
    }
    else {
        printf("IP mismatch!\n");
        write(cfd, buff1, (sizeof(buff1)));
    }
    close(cfd);
    return 0;
}
