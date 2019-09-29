#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
typedef struct node Node;
struct node
{
    char domain[100];
    char ip[16];
    Node *next;
};
Node *getNode()
{
    return (Node *)malloc(sizeof(Node));
}
Node *addNode(Node* head, char domain[], char ip[])
{
    Node *temp = NULL;
    if(head==NULL) {
        head = getNode();
        strcpy(head->domain, domain);
        strcpy(head->ip, ip);
        head->next = NULL;
    }
    else {
        for(temp = head; temp->next != NULL; temp = temp->next);
        Node *n = getNode();
        strcpy(n->domain, domain);
        strcpy(n->ip, ip);
        n->next = NULL;
        temp->next = n;
    }
    return head;
}
int main(int argc, char **argv)
{
    unsigned int len, flag;
    int sockfd, newfd, n = 0;
    struct sockaddr_in servaddr, cliaddr;
    char buff[100];
    struct hostent *he;
    struct in_addr **addr_list;
    Node *table = NULL, *temp = NULL;
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
        flag = 0;
        n = recvfrom(sockfd, buff, sizeof(buff), MSG_WAITALL, (struct sockaddr*)&cliaddr, &len);
        if(n != 0) {
            printf("DNS request for ");
            printf("%s\n", buff);
            for(temp = table; temp != NULL && flag == 0; temp = temp->next) {
                if(strcmp(buff, temp->domain) == 0) {
                    flag = 1;
                    strcpy(buff, temp->ip);
                    printf("Present in table: %s\n", buff);
                }
            }
            if(flag == 0) {
                he = gethostbyname(buff);
                addr_list = (struct in_addr **) he->h_addr_list;
                table = addNode(table, buff, inet_ntoa(*addr_list[0]));
                strcpy(buff, inet_ntoa(*addr_list[0]));
                printf("Added to table: %s\n", buff);
            }
            n = sendto(sockfd, buff, sizeof(buff), MSG_WAITALL, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
        }
    }
    close(sockfd);
    return 0;
}
