#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>

#define BUFFER_SIZE 256

void *serverThread(void *input)
{
    int portNum = *(int *)input;
    /*
     * Write your server thread here.
    **/
    int sockfd;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in servaddr, cliaddr;
	
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(portNum);
	
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	while(1)
	{
		int len, n;
		
		len = sizeof(cliaddr); //len is value/result
		n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE,
					MSG_WAITALL, ( struct sockaddr *) &cliaddr,
					&len);
		buffer[n] = '\0';
		printf("recv from client: %s\n", buffer);
		bzero(buffer, BUFFER_SIZE);	
	}
	close(sockfd);  
}

void *clientThread(void *input){
    /*
     * Write your client thread here.
    **/
    int sockfd;
	char ip[100];
	int num;
	char str_num[10];
	char message[200];
	char temp;
	//char buffer[BUFFER_SIZE];
	struct sockaddr_in	 servaddr;

	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	
	printf("Please enter IP address: ");
	scanf("%s", ip);
	
	printf("Please enter port number: ");
	scanf("%d", &num);

	
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(num);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	while(1)
	{
		int n, len;
		scanf("%c",&temp);
		scanf("%[^\n]",message);
		sendto(sockfd, (const char *)message, strlen(message),
			MSG_CONFIRM, (const struct sockaddr *) &servaddr,
				sizeof(servaddr));
	}
	close(sockfd);
    
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 0;
    }
    int portNum = atoi(argv[1]);
    pthread_t clientTid, serverTid;
    pthread_create(&clientTid, NULL, clientThread, NULL);
    pthread_create(&serverTid, NULL, serverThread, &portNum);
    pthread_join(clientTid, NULL);
    pthread_join(serverTid, NULL);
    return 0;
}
