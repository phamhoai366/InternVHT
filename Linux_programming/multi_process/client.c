#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8888
#define SERVER_IP "127.0.0.1"

int main ()
{
    int cliSoc ;
    struct sockaddr_in serverAddr;
    char buffer [1024];
    // Create client socket
    cliSoc = socket (AF_INET, SOCK_STREAM, 0);
    if (cliSoc  < 0) {
        perror ("Error in socket creation");
        exit (1);
    }

    printf("Client %d socket created.\n", getpid ());

    // Set server address parameters
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons (PORT);
    serverAddr.sin_addr.s_addr = inet_addr (SERVER_IP);
    // Connect to the server
    if (connect (cliSoc , (struct sockaddr*) & serverAddr, sizeof (serverAddr)) < 0) {
        perror("Error in connecting to server");
        exit (1);
    }

    printf ("Connected to server.\n");
    while (1) {
        // Read input from user
        printf ("Client %d - Enter a message: ", getpid ());
        fgets (buffer, sizeof(buffer), stdin);

       
        if (send (cliSoc, buffer, strlen(buffer), 0) < 0) {
            perror ("Error in sending data");
            exit (1);

        }
        // Receive response from the server
        memset (buffer, 0, sizeof(buffer));
        if (recv (cliSoc, buffer, sizeof(buffer), 0) < 0) {
            perror ("Error in receiving data");
            exit (1);
        }
        printf ("Client %d - Server response: %s\n", getpid (), buffer);
    }

    // Close the client socket
    close (cliSoc);
    return 0;
}