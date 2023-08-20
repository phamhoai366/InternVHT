#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define SIZE 1024

void send_file(FILE *fp, int sockfd)
{
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp) != NULL)
    {
        if(send(sockfd, data, sizeof(data), 0)== -1)
        {
            perror("[-] Error in sendung data");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main ()
{
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    FILE *fp;
    char *filename = "text.txt";

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_addr, '\0', sizeof(server_addr));

    if(sockfd < 0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created. \n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = bind(sockfd,(struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e < 0)
    {
    perror("[-]Error in Binding");
    exit(1);
    }
    printf("[+]Binding Successfull.\n");

    e = listen(sockfd, 10);
    if(e == 0)
    {
        printf("[+]Listening...\n");
    }
    else 
    {
        perror("[-]Error in Binding");
        exit(1);
    }
    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd,(struct sockaddr*)&new_addr, &addr_size);

    printf("Send message to IP: %s and port: %i\n", inet_ntoa(new_addr.sin_addr), ntohs(new_addr.sin_port));

    fp = fopen(filename, "r");
    if(fp == NULL)
    {
        perror("[-]Error in reading file. \n");
        exit(1);
    }
    send_file(fp, new_sock);
    printf("[+] File data send successfully. \n");
    close(sockfd);
    printf("[+]Disconnected from the server. \n");
    return 0;    
}


