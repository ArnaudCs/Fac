#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void * f1(void* i){
    printf("Bonjour je suis le thread %d\n", (int) i);
    printf("Calcul en cours du résultat de %d * 5\n", (int) i);
    int n = (int)i*5;
    printf("Résultat : %i\n", n);
    printf("Fin d'utilisation du thread %d\n", (int) i);
    printf("-------------------------------\n");
    pthread_exit(0);
}

int main(int argc, char * argv[]){
    if(argc != 2){
        printf("Erreur arguments : ./programme nbr_threads\n");
        exit(1);
    }

    int n = atoi(argv[1]);

    pthread_t thread[n]; //tableau de threads avec nombre en paramètre
    printf("Bonjour je suis le père\n");
    for(int i = 0; i < n; i++){
        pthread_create(&(thread[i]), NULL, f1, (void*) i);
    }
    for(int j = 0; j < n; j++){
        pthread_join(thread[j], NULL); // pas besoin de pointeurs car on veut juste lire le thread par l'écrire
    }
    printf("Toutes les opérations sont terminées\n");

    return EXIT_SUCCESS;
}