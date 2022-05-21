#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "calcul.h"

struct predicatRdv {  // regroupe les données partagées entres les threads participants aux RdV :
    pthread_mutex_t lock; //création du mutex
    pthread_cond_t cond; //création de la variable conditionnelle
    int sharedData; //création de la ressource partagée
};

struct params {  // structure pour regrouper les paramètres d'un thread. 
    int idThread; // un identifiant de thread, de 1 à N (N le nombre total de theads secondaires)
    int n; //nombre de threads secondaires
    struct predicatRdv *varPartagee; //en paramètre de la fonction thread la struct des variables
};

// fonction associée a chaque thread participant au RdV.
void * participant (void *p) {
    struct params *args = (struct params *) p;
    int wait = args->idThread + rand() % 3; //temps d'attente qui dépend du nombre de thread
    printf("[Thread n°%i] Démmarage du calcul\n", args->idThread);
    calcul(wait); // Simulation d'un long calcul pour le travail avant RdV

    // RdV 
    pthread_mutex_lock(&args->varPartagee->lock); //on bloque pour que le thread concerné fasse ses opérations
    args->varPartagee->sharedData += 1; //on ajout + 1 à l'int qui joue le rôle de ressource partagée
    printf("[Thread n°%i] %i/%i.\n", args->idThread, args->varPartagee->sharedData, args->n);
    if (args->varPartagee->sharedData < args->n) {  // attention : le dernier arrivé ne doit pas attendre. Il doit réveiller tous les autres.
        pthread_cond_wait(&args->varPartagee->cond, &args->varPartagee->lock); //on attend que la condition soit rempli et elle serait 
        //remplie une fois le pthread broadcast affiché
    }
    else if (args->varPartagee->sharedData == args->n) { //si tous les threads ont réalisés leurs opérations
        printf(" -------------------------------------------------------------------\n");
        printf("| [Arrivée du thread n°%i] Point de rendez-vous, réveil des threads |\n", args->idThread);
        printf(" -------------------------------------------------------------------\n");
        pthread_cond_broadcast(&args->varPartagee->cond); //après attente avec le wait on réveille tous les threads
    }
    pthread_mutex_unlock(&args->varPartagee->lock); //on débloque le thread appelant 
    
    wait = rand() % 3; //on effectue une autre attente
    printf("[Thread n°%i] Reprise du travail\n", args->idThread); //reprise du calcul
    calcul(wait); // reprise et poursuite de l'execution.

    pthread_exit(NULL); // fortement recommandé.
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Utilisation: %s nombre_threads\n", argv[0]);
        exit(1);
    }

    // Initialisations 
    pthread_t threads[atoi(argv[1])]; //tableau de threads avec le nombre en argument
    struct params tabParams[atoi(argv[1])]; //tableau de params avec le nombres de threads en argument
    struct predicatRdv predicat; //structure qui contient les données partagées
    predicat.sharedData = 0; //on initialise l'int partagée à 0

    int res;
    if ((res = pthread_mutex_init(&predicat.lock, NULL)) != 0) {
        printf("Erreur : %s\n", strerror(res));
        exit(EXIT_FAILURE);
    }
    if ((res = pthread_cond_init(&predicat.cond, NULL)) != 0) {
        printf("Erreur : %s\n", strerror(res));
        exit(EXIT_FAILURE);
    }

    srand(atoi(argv[1]));  // initialisation de rand pour la simulation de longs calculs
    
    // Création des threads 
    for (int i = 0; i < atoi(argv[1]); i++){
        tabParams[i].idThread = i + 1;
        tabParams[i].n = atoi(argv[1]);
        tabParams[i].varPartagee = &predicat; 

        if (pthread_create(&threads[i], NULL, participant, (void*)&tabParams[i]) != 0) {
            //on passe un tableau qui contient l'id du thread appelant, le nombre de threads demandé, et la vairbale partagée
            perror("Erreur : problème lors de la création du thread ");
            exit(1);
        }
    }

    // Attente de la fin des  threads. Partie obligatoire 
    for (int i = 0; i < atoi(argv[1]); i++) { //attente de la fin de tous les threads crées
        pthread_join(threads[i], NULL);
    }
    
    printf(" ----------------------------------------------- \n");
    printf("| Thread principal fin des threads secondaires. |\n");
    printf(" ----------------------------------------------- \n");

    // terminer "proprement". 
    pthread_cond_destroy(&predicat.cond);
    pthread_mutex_destroy(&predicat.lock);
    return 0;
}
 