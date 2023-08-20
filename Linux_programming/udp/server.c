#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>         //read(), write(), close()


int main(){
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char server_msg[1024], client_msg[1024];

    //clean buffer
    memset(server_msg, '\0', sizeof(server_msg));
    memset(client_msg, '\0', sizeof(client_msg));

    // Create UDP socket:
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sockfd < 0){
        printf("Error while create socket");
    }
    else{
        printf("socket created successfully\n");
    }

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4455);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Receive client's message:
    int client_struct_length = sizeof(client_addr);
    if (recvfrom(sockfd, client_msg, sizeof(client_msg), 0, (struct sockaddr*)&client_addr, &client_struct_length) < 0){
        printf("Couldn't receive\n");
        return -1;
    }

    printf("Received message from IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf("Msg from client: %s\n", client_msg);
    
    // Respond to client:
    strcpy(server_msg, client_msg);
    
    if (sendto(sockfd, server_msg, strlen(server_msg), 0, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0){
        printf("Can't send\n");
        return -1;
    }
    
    // Close the socket:
    close(sockfd);
    
    return 0;
}