#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8888

int main ()
{
    int serSoc, cliSoc ;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof (clientAddr);
    char buffer [1024];
    int clientCount = 0;

    // Create server socket
    serSoc = socket(AF_INET, SOCK_STREAM, 0);
    if (serSoc < 0) {
        perror ("Error in socket creation");
        exit (1);
    }

    printf ("Server socket created.\n");

    // let us Set server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons (PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serSoc, (struct sockaddr*) & serverAddr, sizeof (serverAddr)) < 0) {
        perror ("Error in binding");
        exit (1);
    }

    // Start listening for incoming connections
    if (listen (serSoc, 5) == 0) {
        printf ("Listening for connections...\n");
    } else {
        perror ("Error in listening");
        exit (1);
    }

    while (1) {
        // Let us  Accept for a  client connection
        cliSoc  = accept (serSoc, (struct sockaddr* )& clientAddr, &addrSize);
        if (cliSoc  < 0) {
            perror("Error in accepting connection");
            exit (1);
        }

        // Increment the client count and display client information
        clientCount++;
        printf("Accepted connection from Client %d: %s:%d\n", clientCount, inet_ntoa (clientAddr.sin_addr), ntohs (clientAddr.sin_port));
        printf ("Total clients connected: %d\n", clientCount);
       
        pid_t pid = fork ();
        if (pid == 0) {
            // Child process         
            close (serSoc);

            // Receive messages from the client
            while (1) {
                memset (buffer, 0, sizeof(buffer));
                if (recv (cliSoc , buffer, sizeof (buffer), 0) < 0) {
                    perror ("Error in receiving data");
                    exit(1);
                }

                printf ("Received data from Client %d: %s\n", clientCount, buffer);

                // Echo the received message back to the client
                if (send (cliSoc , buffer, strlen (buffer), 0)) {
                // Parent process
        
                    close(cliSoc );
                } else {
                    perror ("Error in forking");
                    exit (1);
                }
            }
        }
    }

    // Close the server socket
    close (serSoc);

    return 0;
}
