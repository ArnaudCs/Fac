#include<stdio.h>
#include<stdlib.h>
#include"tubesem.h"

Semaphore C;

//initialiser les sémaphores
void initsem(Semaphore S, int N){
    int i;
    char c = 'a';
    pipe(S);
    for(int i = 1; i<=N; i++){
        write(S[1], &c, 1);
    }
}

//P sur le semaphore S, pendre un jeton 'a'
void P (Semaphore S){
    char c;
    read(S[0], &c, 1);
}

void V (Semaphore S){
    char c ='a';
    write(S[1], &c, 1);
}

void attente(int N){
    sleep(rand()%N);
}

void message(int i, char* s){
    #define colonne 20
    int Nb, j;
    Nb = (i-1)*colonne;
    for(j=0; j<Nb; j++){
        putchar(' ');    
    }
    printf("%s\n", s);
    fflush(stdout);
}

void voiture(int n, int duree){
    if(fork() == 0){
        attente(duree);
        message(n, "Arrivee");
        P(C);
        message(n, "Traversee");
        attente(duree);
        message(n, "Depart");
        V(C);
        exit(0);
    }
}

main(){
    void voiture(int, int);
    printf("%20s%20s%20s\n", "Voiture 1", "Voiture 2", "Voiture 3");

    initsem(C, 1);

    voiture(1,3);
    voiture(2,1);
    voiture(3,3);

    for(int i=1; i<=3; i++){
        wait(0);
    }
}