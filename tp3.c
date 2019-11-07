#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>

int main( int argc, char * argv[]){
    int tube[2], cr, no_err;
    pipe(tube);
    struct timeval tdebv, tfinv;
    float tempsmoyfils, tempsmoy = 0, temp;

    if(argc != 4){
        fprintf(stderr,"Mauvais nombre d'arguments \n");
        fprintf(stderr,"TP3.c : <nb de ps fils> <nb d'exec> <commande> \n");
        exit(1);
    }
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    char c[strlen(argv[3])];
    strcpy(c, argv[3]);

    for(int i = 0; i < n; i++){
        if((no_err = fork()) == 0){
         /* Le processus est un fils */
            if(gettimeofday(&tdebv, NULL)){
                fprintf(stderr,"Problème récupération du temps\n");
                exit(2);
            }
            for(int j = 0; j < k; j++){
                if((no_err = fork())  == 0){
                    /* Le processus est un petit fils */
                    if(execlp(c,c,NULL) == -1)
                       fprintf(stderr,"Probleme sur le execl\n");
                          
                }else if(no_err == -1){
                    fprintf(stderr,"Erreur sur le fork n°2\n");
                    exit(3);
                }
            }
            while(wait(&cr) != -1);
            if(gettimeofday(&tfinv, NULL)){
                fprintf(stderr,"Problème récupération du temps\n");
                exit(4);
            }
            tempsmoyfils = (float)(( tfinv.tv_sec * 1000000 + tfinv.tv_usec ) - ( tdebv.tv_sec * 1000000 + tdebv.tv_usec ))/k;
            write(tube[1], &tempsmoyfils, sizeof(float));
            exit(0);
        }else if(no_err == -1){
            fprintf(stderr,"Erreur sur le fork n°1\n");
            exit(5);
        }
    }
    while(wait(&cr) != -1);
    close(tube[1]);
    while(read(tube[0], &temp, sizeof(float))){
        tempsmoy += temp;
    }
    close(tube[0]);
    tempsmoy /= n;
    tempsmoy /= 1000000;
    printf("Temps Moyen d'execution de %s = %.3f \n", c, tempsmoy);
    exit(0);
}