//Question 5b

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

//Structure for the ACK packet
struct TFTPAckPacket {
    uint16_t opcode;      // Opcode for ACK
    uint16_t block_number; // Block number
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

//Function to send a file as a single DAT packet and receive its ACK
void sendSingleDAT(int sockfd, struct sockaddr *server_addr, socklen_t server_addr_len, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file for reading");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    //Prepare the DAT packet
    char dat_buffer[MAX_BUFFER_SIZE];
    dat_buffer[0] = 0x00;  // Opcode (2 bytes)
    dat_buffer[1] = TFTP_OPCODE_DATA;

    //Set the block number (assuming it is 1 for simplicity)
    uint16_t block_number = htons(1);
    memcpy(dat_buffer + 2, &block_number, sizeof(uint16_t));

    //Read the file content into the DAT packet
    size_t read_size = fread(dat_buffer + 4, 1, MAX_BUFFER_SIZE - 4, file);
    fclose(file);

    //Send the DAT packet
    if (sendto(sockfd, dat_buffer, 4 + read_size, 0, server_addr, server_addr_len) == -1) {
        perror("Error sending DAT packet");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    //Receive the ACK packet
    struct TFTPAckPacket ack_packet;
    ssize_t recv_length = recvfrom(sockfd, &ack_packet, sizeof(ack_packet), 0, server_addr, &server_addr_len);

    if (recv_length == -1) {
        perror("Error receiving ACK packet");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    //Check if the received ACK is for the correct block number
    if (ack_packet.opcode == htons(TFTP_OPCODE_ACK) && ack_packet.block_number == block_number) {
        printf("File sent successfully.\n");
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
    // Send the file as a single DAT packet and receive its ACK
    sendSingleDAT(sockfd, res->ai_addr, res->ai_addrlen, filename);
    //Free the linked list when done with it
    freeaddrinfo(res);
    //Close the socket when done
    close(sockfd);

    //puttftp(serverhost, filename);
    //gettftp(serverhost,filename);

    return EXIT_SUCCESS;
}
