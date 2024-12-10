//Question 2

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

void gettftp(char *serveurhost,char *filename){
    
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

    struct addrinfo hints,*res;
    memset(&hints, 0, sizeof(struct addrinfo));
    //IPv4
    hints.ai_family = AF_INET;      
    //Datagram socket for TFTP
    hints.ai_socktype = SOCK_DGRAM; 

    int status = getaddrinfo(serverhost, "69", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    //Extract the address of the server
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipv4->sin_addr), ip_str, INET_ADDRSTRLEN);

    //Print the Server IP to verify 
    char message[128];
    snprintf(message, sizeof(message),"Server IP: %s\n", ip_str);
    write(STDOUT_FILENO,message, strlen(message));
    
    //Free the memory allocated
    freeaddrinfo(res); 

    puttftp(serverhost, filename);
    //gettftp(serverhost,filename);

    return EXIT_SUCCESS;
}
