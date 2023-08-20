#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>         //read(), write(), close()


void write_file(int sockfd, struct sockaddr_in addr)
{

  char* filename = "test2.txt";
  int n;
  char buffer[1024];
  socklen_t addr_size;

  // Creating a file.
  FILE* fp = fp = fopen(filename, "w");

  // Receiving the data and writing it into the file.
  while (1)
  {
    addr_size = sizeof(addr);
    n = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, &addr_size);

    if (strcmp(buffer, "END") == 0)
    {
        break;
    }

    printf("[RECEVING] Data: %s", buffer);
    fprintf(fp, "%s", buffer);
    bzero(buffer, 1024);
  }

    fclose(fp);
}

int main(){
    int socket_desc;
    struct sockaddr_in server_addr;
    int server_struct_length = sizeof(server_addr);
    char client_message[1024];

    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, 0);

    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    else{
        printf("Socket created successfully\n");
    }

    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4455);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
        // Get input from the user:
    printf("Enter message: ");
    fgets(client_message, 1024, stdin);
    
    // Send the message to server:
    if(sendto(socket_desc, client_message, strlen(client_message), 0, (struct sockaddr*)&server_addr, server_struct_length) < 0){
        printf("Unable to send message\n");
        return -1;
    }

    // Receive the server's response:
    write_file(socket_desc, server_addr);

    close(socket_desc);
    return 0;
}