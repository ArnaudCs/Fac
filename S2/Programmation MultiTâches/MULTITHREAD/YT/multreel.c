#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* threadfunction(void * arg){ //le paramètre est un void * pointeur qui sera casté dans la fonction
    printf("Thread numéro %i démarré\n", (int *) arg);
}

int main(){
    printf("MultiThreading en temps réel\n");
    pthread_t tid[8];
    int* local_id[8]; //allocation dynamique pas obligatoire, id locaux 
    //configuration et lancement des threads
    for(int i =0; i<8; i++){
        local_id[i] = i;
        pthread_create(tid+i, 0, &threadfunction, local_id+i); //tids+1 pointeur qui pointe vers le premier id
                                                         //pointeur sur une fonction et on envoi en argument la case 
                                                         //numéro i du tableau des id
    }

    //attente de la fin des threads
    for(int i = 0; i<8; i++){ //fonction bloquante tant que le thread i n'a pas terminé
        pthread_join(tid[i], 0);
    }

    return 0;
}

