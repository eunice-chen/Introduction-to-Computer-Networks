#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LENGTH 16

struct Data_sequence{
    int seq_num;
    char Data[LENGTH];
};
struct ack_pkt{
    int seq_num;
};
// ----------------------------
// for simulation purposes
// ----------------------------
bool check = false;  
int rec;
// ----------------------------

int main(int argc , char *argv[])
{ 
    //===========================================
	// Todo: Create TCP socket and TCP connection
	//===========================================

    int sockfd, port_num = atoi(argv[1]); 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int count = 1;   
    if(sockfd == -1) {
        perror("Fail to create the respective socket\n");
        exit(EXIT_FAILURE);
    }

    //Specifying the address for the socket
    struct sockaddr_in serverInfo; 
    serverInfo.sin_family = AF_INET; 
    serverInfo.sin_port = htons(port_num); 
    serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");  //convert number and dots into network binary order. 

    if(connect(sockfd, (struct sockaddr*) &serverInfo, sizeof(serverInfo)) != 0){
        perror("--socket connection failed--\n");
        exit(EXIT_FAILURE);
    }
    else { printf("Connection succesful\n"); }
    

    //========================
	// Receive data / send ack
	//========================
    struct Data_sequence data_seq;
    struct ack_pkt ACK;
    data_seq.seq_num = 0; 
    
    while(1){
        //=================================================================
        // Todo: 1. receive data and send ACKs with correct sequence number
        //       2. simulate packet loss (or you can implement in server.c)
        // Required format: 
        //       received: seq_num = [sequence number]
        //       loss: seq_num = [seq_num]
        //=================================================================
        
        recv(sockfd, &data_seq.Data, LENGTH, 0);
        data_seq.seq_num = atoi(data_seq.Data); 

        if (check == true && (data_seq.seq_num == 8 || data_seq.seq_num == 13)){
            check = false; 
        }
        if (( count != 0 && data_seq.seq_num == 8 || data_seq.seq_num == 13) ){
            printf("lost: seq_num: %d\n", data_seq.seq_num);
            ACK.seq_num = rec = data_seq.seq_num;
            check = true;  
        }
        if(check == true){
            if (count != 0){
                send(sockfd, &data_seq.Data, LENGTH, 0);
                count--;
            }
            else{
                printf("received: seq_num = %d\n", data_seq.seq_num);
                ACK.seq_num = rec; 
                send(sockfd, &data_seq.Data, LENGTH, 0);
            }
        }
        else {
            printf("received: seq_num = %d\n", data_seq.seq_num);
            ACK.seq_num = data_seq.seq_num; 
            send(sockfd, &data_seq.Data, LENGTH, 0);
        }
        
    }
    
    close(sockfd);

    return 0;
    
}
