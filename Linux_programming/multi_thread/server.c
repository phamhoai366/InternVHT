#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>//for sockaddr_in
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8 /*maximum number of client connections*/

char buf[MAXLINE];
int n;

void do_service(int sd2){
	int alen; /* length of address */
	char buf_recv[4096], buf_send[4096]; /* buffer for string the server send 1000 */
	char file_buffer[1000], f_buffer[1000];
	int n;
	FILE *fp;
    printf("\nRequesting client to send file name: ");
    sprintf(buf_send, "Please enter the file name: ");
    send(sd2, buf_send, strlen(buf_send), 0);

    printf("\nReceiving the file name: ");
    n = recv(sd2, buf_recv, 1000, 0);
    buf_recv[n] = '\0';
    printf("%s\n", buf_recv);
    fflush(stdout);

    printf("Checking FILE: %s exists or not...\n", buf_recv);
    if((fp = fopen(buf_recv, "r")) == NULL){
        sprintf(buf_send, "File could not be found!!!");
        exit(0);
    } else{
        printf("\nFile found!!!\n");
        sprintf(buf_send, "File found!!!\n");
        send(sd2, buf_send, strlen(buf_send), 0);
    }

    printf("Sending the file content to client....\n");
    while(!feof(fp))//loops till eof
    {
        fgets(f_buffer, 1000, fp);//extracts 1000 chars from file
        if (feof(fp))
        break;
        strcat(file_buffer, f_buffer);
    }

    fclose(fp);
    send(sd2, file_buffer, strlen(file_buffer), 0);//sends 1000 extracted byte
    close(sd2);
    printf("[Server] Connection with Client closed. Server will wait now...\n");
}//end of do_service

//do service function executed by child
int  curr_sd, n;
void* body(void* arg)
{
    struct sockaddr_in c_add;
    int addrlen;
    int i, l;
    int base_sd = *(int*) arg;
    int sd;
    while (1) {
    //pthread_mutex_lock(&m_acc);
    sd = accept(base_sd, (struct sockaddr*)&c_add, &addrlen);
    //pthread_mutex_unlock(&m_acc);
    printf("\nserver connected now sd = %d", sd);
    do_service(sd);
    close(sd);
    }
}

//creating socket and listening
int connectbody(){
    int base_sd2;
    struct sockaddr_in c_add, servaddr;

    if ((base_sd2 = socket(AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        exit(2);
    }

    //preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    //bind the socket
    bind (base_sd2, (struct sockaddr *) &servaddr, sizeof(servaddr));

    //listen to the socket by creating a connection queue, then wait for clients
    listen (base_sd2, LISTENQ);

    printf("%s\n","Server running...waiting for connections.");

    return base_sd2;
}



int main (int argc, char **argv){
    //Create a socket and listen
    int base_sd;
    base_sd = connectbody();

    //accepting the maximum queud connections
    pthread_t t[10];
    struct sockaddr_in c_add;
    socklen_t addrlen;
    addrlen = sizeof(c_add);

    int *info = malloc(sizeof(info));
    *info = base_sd;

    int i;
    for (i=0 ;i<10 ;i++ ){
        printf("\n\nthread id=t[%d]\n\n\n\n\n",i);
        printf ("%s\n","thread created for dealing with client requests");
        pthread_create(&t[i], NULL, body, info);
        pause();
    }//end of infinite for loop

    for(i = 0; i < 10; i++){
        pthread_join(t[i], NULL);
    }
}
