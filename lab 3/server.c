#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LENGTH 16
int ROUND = 50;

//======================================
// Data sequence
// sample sequence packet:
//    seq_num: sequence number
//    Data: data
// you can add another variables if need
//======================================
struct Data_sequence{
    int seq_num;
    char Data[LENGTH];
};

//================================
// ACK packet with sequence number
//================================
struct ack_pkt{
    int seq_num;
};

//======================
// Parameter declaration
//======================
int sockfd = 0;
struct sockaddr_in serverInfo;
int clientSockfd;
struct sockaddr_in clientInfo;
struct Data_sequence data_seq;
struct ack_pkt ACK;
int cwnd = 1;
int ssthresh = 8;
bool check = false;
int last_seq;

void sender(){
    //===========================================
    // Write your send here
    // Todo: 1. send cwnd sequences of data that
    //          starts with right sequence number
    // Required format: 
    //       send: seq_num = [sequence number]
    //===========================================
    
    for(int i=0; i<cwnd; i++)
    {
        send(clientSockfd, &data_seq.Data, LENGTH, 0);
        printf("send: seq_num = %d\n", data_seq.seq_num);
        data_seq.seq_num++;
    }
    
    if(check!= false){
    	data_seq.seq_num = last_seq;
    	check = false;
    }
}

void receiver()
{
    //====================================================
    // Write your recv here
    // Todo: 1. receive ACKs from client
    //       2. detect if 3-duplicate occurs
    //       3. update cwnd and ssthresh
    //       4. remember to print the above information
    // Required format: 
    //       ACK: [sequence number]
    //       3-duplicate ACKs: seq_num = [sequence number]
    //       cwnd = [cwnd], ssthresh = [ssthresh]
    //====================================================
    int pre_ACK;
    int count = 0;

    for(int i=0; i<cwnd; i++){
        recv(clientSockfd, &data_seq.Data, LENGTH, 0);
        ACK.seq_num = atoi(data_seq.Data);
        if (pre_ACK == ACK.seq_num){
            count++; 
        }
        
        if (count == 3){
            printf("3 duplicate ACKs: seq_num = %d, ssthresh = %d\n", ACK.seq_num, ssthresh);
            check = true;
        }
        pre_ACK = ACK.seq_num;
        printf("ACK:seq_num: %d\n", ACK.seq_num);
    }
    count = 0;

    if (check != false){
        ssthresh /= 2; 
        cwnd = 1; 
        printf("state: slow start\n"); 
        printf("cwnd = %d, ssthresh = %d\n", cwnd, ssthresh);
        last_seq = data_seq.seq_num;
        data_seq.seq_num = pre_ACK; 
    } 
    else if (cwnd >= ssthresh){
    	printf("state: congestion avoidance\n");
        printf("cwnd = %d, ssthresh = %d\n", cwnd, ssthresh);
        cwnd += 1;
    }   
    else {
        cwnd *= 2;
    }
}

int main(int argc, char *argv[])
{
    //===========================================
	// Todo: Create TCP socket and TCP connection
	//===========================================
    //char inputBuffer[256] = {};
    //char message[] = {"Hi, this is server.\n"};
    //clientSockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1){
        printf("fail to create a socket!");
    }

   // struct sockaddr_in serverInfo, clientInfo;
    int addrlen = sizeof(clientInfo);
   // bzero(&serverInfo, sizeof(serverInfo));

    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(8080);
    bind(sockfd,(struct sockaddr*) &serverInfo,sizeof(serverInfo));
    listen(sockfd, 5);
    clientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
    //============================================================================
	// Start data transmission
    // To be simple, we receive ACKs after all cwnd sequences of data are sent.
    // Thus we can not react to 3-duplicate immediately, which is OK for this lab.
	//============================================================================
    printf("state: slow start\n");
    printf("cwnd = %d, ssthresh = %d\n", cwnd, ssthresh);
    while(ROUND--){
        
        sender();
        receiver();
        
    }
    close(sockfd);
    return 0;
}
