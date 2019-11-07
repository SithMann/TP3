#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <math.h>

int main( int argc, char * argv[]){

    if(argc != 4){
        fprintf(stderr,"Mauvais nombre d'arguments \n");
        fprintf(stderr,"TP3.c : <nb de ps fils> <nb d'exec> <commande> \n");
        exit(1);
    }
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    char c[strlen(argv[3])];
    strcpy(c, argv[3]);

    int tube[2], cr, no_err, i = 0, j, pid,x;
    pipe(tube);
    struct timeval tdebv, tfinv;
    float tempsmoy = 0, temp, lesTemps[n], tempsfils;

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
                    if(execlp(c,c,NULL) == -1){
                        fprintf(stderr,"Probleme sur le execl\n");
                        exit(8);
                    }
                          
                }else if(no_err == -1){
                    fprintf(stderr,"Erreur sur le fork n°2\n");
                    exit(3);
                }
                while((pid = wait(&cr)) != -1);
                /* En cas de terminaison anormale */
                if(cr != 0){
                    /*Interruption par signal, les bits de poids faibles contiennent le numero, les autres sont à 0 */
                    if(cr>>8 == 0)
                        fprintf(stderr, "Fin du processus de la commande interrompu par un signal, numero de signal = %d\n ",cr);
                    /* exit du fils != 0, code retour du fils dans les 8 bits de poids forts, les autres sont à 0*/
                    else
                        fprintf(stderr, "Fin du processus de la commande avec un cr != 0, numero de cr = %d\n", cr>>8);
                    exit(5);
                }else{
                    if(gettimeofday(&tfinv, NULL)){
                        fprintf(stderr,"Problème récupération du temps\n");
                        exit(4);
                    }
                    tempsfils = (float)(( tfinv.tv_sec * 1000000 + tfinv.tv_usec ) - ( tdebv.tv_sec * 1000000 + tdebv.tv_usec ));
                    write(tube[1], &tempsfils, sizeof(float));
                    exit(0);
                }
            }
        }else if(no_err == -1){
            fprintf(stderr,"Erreur sur le fork n°1\n");
            exit(6);
        }
    }
    while(wait(&cr) != -1);
    /* En cas de terminaison anormale */
    if(cr != 0){
        /*Interruption par signal, les bits de poids faibles contiennent le numero, les autres sont à 0 */
        if(cr>>8 == 0)
            fprintf(stderr, "Fin du processus fils interrompu par un signal, numero de signal = %d\n ", cr);
        /* exit du fils != 0, code retour du fils dans les 8 bits de poids forts, les autres sont à 0*/
        else
            fprintf(stderr, "Fin du processus fils avec un cr != 0, numero de cr = %d\n", cr>>8);
        exit(7);
    }else{
        close(tube[1]);
        while(read(tube[0], &temp, sizeof(float))){
            tempsmoy += temp;
            lesTemps[i] = temp/1000000;
            i++;
        }
        close(tube[0]);
        tempsmoy /= n;
        tempsmoy /= 1000000;
        printf("Temps Moyen d'execution de %s = %.4f \n\n", c, tempsmoy);

        printf("\tN° Processus i\t|\tMi (en sec)\n");
        printf("-------------------------------------------\n");
        for(j = 0; j < n; j++){
            printf("\t%d\t\t|\t%.4f \n", j, lesTemps[j]);
        }

        printf("\nLe résultat final est le milieu du tableau, tab[%d] = %.4f sec\n\n", i/2, lesTemps[i/2]);
        exit(0);
    }
}