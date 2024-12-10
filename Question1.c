//Question 1

#include <stdlib.h>
#include <stdio.h>

void gettftp(char *serveurhost,char *filnemae){
    
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

    puttftp(serverhost, filename);
    gettftp(serverhost,filename);

    return EXIT_SUCCESS;
}