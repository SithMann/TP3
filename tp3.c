#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main( int argc, char * argv[]){

    if(argc != 4){
        fprintf(stderr,"Mauvais nombre d'arguments \n");
        exit(1);
    }

    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    char * c = argv[3];

    for(int i = 0; i < n; i++){
        switch(fork()){
            case -1:fprintf(stderr,"Erreur sur le fork\n");
                    exit(1);
            case 0: /* Le processus est un fils */
                    for(int j = 0; j < k; j++)
                        execl(c,c,NULL);
            default: /* Le processus est le père */
        }
    }

}