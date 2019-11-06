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
                    if(!gettimeofday(&tdeb, )){
                        fprintf(stderr,"Problème récupération du temps\n");
                        exit(2);
                    }
                    for(int j = 0; j < k; j++){
                        switch(fork()){
                            case -1:fprintf(stderr,"Erreur sur le fork\n");
                                    exit(3);
                            case 0: /* Le processus est un petit fils */
                                    switch(fork()){
                                        case -1:fprintf(stderr,"Erreur sur le fork\n");
                                            exit(4);
                                        case 0: execl(c,c,NULL);
                                        default:
                                    }
                            default: /* Le processus est un fils */
                        }
                    }
                    while(wait(&cr) == -1);
                    if(!gettimeofday(&tfin, )){
                        fprintf(stderr,"Problème récupération du temps\n");
                        exit(5);
                    }
            default: /* Le processus est le père */
        }
    }

}

// prendre le temps avant l'exec des k commandes
// diviser par le nombre de commandes à la fin
// faire un exec.h comme Mathieu c'est smart