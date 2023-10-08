// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// Driver code
int main(int argc, char *argv[]) {
	if (argc < 2)
    	{
        	return 0;
    	}
    	int portNum = atoi(argv[1]);
	int sockfd;
	char buffer[1024];
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
		n = recvfrom(sockfd, (char *)buffer, 1024,
					MSG_WAITALL, ( struct sockaddr *) &cliaddr,
					&len);
		buffer[n] = '\0';
		printf("recv from client: %s\n", buffer);
		bzero(buffer, 1024);	
	}
	close(sockfd);
	return 0;
}

