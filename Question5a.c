//Question 5a

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define TFTP_OPCODE_RRQ 1
#define MAX_BUFFER_SIZE 516
#define TFTP_OPCODE_WRQ 2
#define TFTP_OPCODE_DATA 3
#define TFTP_OPCODE_ACK 4
#define RRQ_MODE "octet"
#define SEND_ERROR "Error sending RRQ packet"

//Structure for the RRQ 
struct TFTPWriteRequestPacket {
    uint16_t opcode;   
    char filename[512]; 
    char mode[10];      
};

//Function to send a WRQ packet to the server
void sendWRQ(int sockfd, struct sockaddr *server_addr, const char *filename) {
    char wrq_buffer[MAX_BUFFER_SIZE];
    //Opcode (2 bytes)
    wrq_buffer[0] = 0x00;  
    wrq_buffer[1] = TFTP_OPCODE_WRQ;
    //Filename
    memcpy(wrq_buffer + 2, filename, strlen(filename));  
    //Null-terminate the filename
    wrq_buffer[2 + strlen(filename)] = 0;
    //Mode
    memcpy(wrq_buffer + 2 + strlen(filename) + 1, RRQ_MODE, strlen(RRQ_MODE));
    //Null-terminate the mode
    wrq_buffer[2 + strlen(filename) + 1 + strlen(RRQ_MODE)] = 0;
    int wrq_length = 2 + strlen(filename) + 1 + strlen(RRQ_MODE) + 1;

    write(STDOUT_FILENO, wrq_buffer, wrq_length);
    write(STDOUT_FILENO, "\n", 1);

    //Send the WRQ packet
    if (sendto(sockfd, wrq_buffer, wrq_length, 0, server_addr, sizeof(struct sockaddr)) == -1) {
        perror(SEND_ERROR);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

void puttftp(char *serveur, char *filename){
    
}

int main(int argc,char *argv[]){
    if (argc != 3){
        //error if number of arguments is different than 3
        fprintf(stderr, "ERROR : Usage: %s <serverhost> <filename>\n", argv[0]);
        exit(EXIT_FAILURE);    
    }

    char *serverhost = argv[1];
    char *filename = argv[2];

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(struct addrinfo));
    //Use IPv4
    hints.ai_family = AF_INET;    
    //Datagram socket for TFTP  
    hints.ai_socktype = SOCK_DGRAM; 

    int status = getaddrinfo(serverhost, "1069", &hints, &res); 
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    //Create a socket and verify if it has been created
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("Socket creation error");
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    //Send WRQ to the server
    sendWRQ(sockfd, res->ai_addr, filename);
    //Free the linked list when done with it
    freeaddrinfo(res);
    //Close the socket when done
    close(sockfd);

    //puttftp(serverhost, filename);
    //gettftp(serverhost,filename);

    return EXIT_SUCCESS;
}
