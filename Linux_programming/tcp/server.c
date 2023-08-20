#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4455

int main(){
    int sockfd;
    struct sockaddr_in server_addr;
    
    int new_socket;
    struct sockaddr_in new_addr;

    socklen_t addr_size;
    char buffer[1024];

    // create a socket file 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_addr, '\0', sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // bind the address
    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // listen
    listen(sockfd, 5);
    addr_size = sizeof(new_addr);

    // accept the connect
    new_socket = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
    strcpy(buffer, "Hello");
    //send the data
    send(new_socket, buffer, strlen(buffer), 0);

    // close(sockfd);

    return 0;
}
