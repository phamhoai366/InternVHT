#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SIZE 1024

void write_file(int sockfd)
{
    int n; 
    FILE *fp;
    char *filename = "text2.txt";
    char buffer[SIZE];

    fp = fopen(filename, "w");
    if(fp == NULL)
    {
        perror("[-]Error in creating file.");
        exit(1);
    }
    while(1)
    {
        n = recv(sockfd, buffer, SIZE, 0);
        if(n <= 0)
        {
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
}

int main()
{
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_addr, '\0', sizeof(server_addr));

    if(sockfd<0)
    {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created. \n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e == -1)
    {
        perror("[-]Error in Connecting");
        exit(1);
    }
    printf("[+]Connected to server.\n");
    
    write_file(sockfd);
    printf("[+]Data written in the text file \n");

    close(sockfd);

    return 0;
}