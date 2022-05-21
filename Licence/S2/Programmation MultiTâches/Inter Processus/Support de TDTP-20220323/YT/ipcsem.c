#include "ipcsem.h"

int Num;

void creersem(key_t cle, int N){
    Num = semget(cle, N, 0600 | IPC_CREAT);
    if(Num == -1){
        perror("Problème lors de la création du sémaphore\n");
    }
}

void initsem(int N, int V){
    semctl(Num, N, SETVAL, V);
}

void P(int N){
    struct sembuf Tabop;
    Tabop.sem_num = N;
    Tabop.sem_op = -1;
    Tabop.sem_flg = 0; //bloquant

    semop(Num, &Tabop, 1);
}

void V(int N){
    struct sembuf Tabop;
    Tabop.sem_num = N;
    Tabop.sem_op = 1;
    Tabop.sem_flg = 0; //bloquant

    semop(Num, &Tabop, 1);
}

void detruiresem(){
    semctl(Num, 0, IPC_RMID, 0);
}

void attente (int N){
    sleep(rand() % N);
}