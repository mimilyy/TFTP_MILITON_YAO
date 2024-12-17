//Question 4b

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
#define TFTP_OPCODE_RRQ 1
#define TFTP_OPCODE_DATA 3
#define TFTP_OPCODE_ACK 4
#define RRQ_MODE "octet"
#define SEND_ERROR "Error sending RRQ packet"

//Structure for the RRQ 
struct TFTPRequestPacket {
    uint16_t opcode;   
    char filename[512]; 
    char mode[10];      
};

//Structure for the Data (DAT) packet
struct TFTPDataPacket {
    uint16_t opcode;      // Opcode for DAT
    uint16_t block_number; // Block number
    char data[512];        // Data
};

//Structure for the ACK packet
struct TFTPAckPacket {
    uint16_t opcode;      // Opcode for ACK
    uint16_t block_number; // Block number
};

//Function to send a RRQ packet to the server
void sendRRQ(int sockfd, struct sockaddr *server_addr, const char *filename) {
    char rrq_buffer[MAX_BUFFER_SIZE];
    //Opcode (2 bytes)
    rrq_buffer[0] = 0x00;  
    rrq_buffer[1] = 0x01;
    //Filename
    memcpy(rrq_buffer + 2, filename, strlen(filename));  
    //Null-terminate the filename
    rrq_buffer[2 + strlen(filename)] = 0;
    // Mode
    memcpy(rrq_buffer + 2 + strlen(filename) + 1, RRQ_MODE, strlen(RRQ_MODE));
    //Null-terminate the mode
    rrq_buffer[2 + strlen(filename) + 1 + strlen(RRQ_MODE)] = 0;
    int rrq_length = 2 + strlen(filename) + 1 + strlen(RRQ_MODE) + 1;

    write(STDOUT_FILENO, rrq_buffer, rrq_length);
    write(STDOUT_FILENO, "\n", 1);

    //Send the RRQ packet
    if (sendto(sockfd, rrq_buffer, rrq_length, 0, server_addr, sizeof(struct sockaddr)) == -1) {
        perror(SEND_ERROR);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

void receiveDAT(int sockfd, struct sockaddr *server_addr, socklen_t server_addr_len, const char *filename) {
    char data_buffer[MAX_BUFFER_SIZE];

    //Receive the DAT packet from the server
    ssize_t recv_length = recvfrom(sockfd, data_buffer, MAX_BUFFER_SIZE, 0, server_addr, &server_addr_len);

    //If error
    if (recv_length == -1) {
        perror("Error receiving DAT packet");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    //ACK packet
    char block[MAX_BUFFER_SIZE];
    block[0] = 0x00;
    block[1] = 0x04;
    //Getting the Block number
    memcpy(block + 2, data_buffer + 2, 2); 
    if (sendto(sockfd, block, 4, 0, server_addr, server_addr_len) == -1) {
        perror("Error sending ACK");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    //Open the file for writing in binary mode
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file for writing");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    //Write the Data into the file
    fwrite(data_buffer + 4, 1, recv_length - 4, file);
    //Close the file
    fclose(file);
}

//Main function for the gettftp program
void gettftp(char *serverhost, char *filename) {
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

    //Send RRQ to the server
    sendRRQ(sockfd, res->ai_addr, filename);
    //Receive DAT packet from the server and send ACK
    receiveDAT(sockfd, res->ai_addr, res->ai_addrlen, filename);
    //Free the linked list when done with it
    freeaddrinfo(res); 
    //Close the socket when done
    close(sockfd);     
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

    //puttftp(serverhost, filename);
    gettftp(serverhost,filename);

    return EXIT_SUCCESS;
}
