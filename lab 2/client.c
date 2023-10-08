// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 8080
//#define MAXLINE 1024

// Driver code
int main() {
	int sockfd;
	char ip[100];
	int num;
	char str_num[10];
	char message[200];
	char temp;
	char buffer[1024];
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
	return 0;
}

