#include <iostream>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "calcul.h"

struct predicatRdv {
// regrouoes les donn�e partag�es entres les threads participants aux RdV :
  int wait;
  pthread_mutex_t* verrou;
  pthread_cond_t* cond;
};

struct params {
  // structure pour regrouper les param�tres d'un thread. 
  int idThread; // un identifiant de thread, de 1 � N (N le nombre
		// total de theads secondaires
  int N;
  struct predicatRdv * varPartagee;
};

// fonction associ�e a chaque thread participant au RdV.

void * participant (void * p){ 
    struct params* args = (struct params*) p;
    struct predicatRdv* predicat = args->varPartagee;

    // On lock 
    printf("Thread n°%i : je me mets en attente du verrou\n", args->idThread);
    pthread_mutex_lock(args->verrou);
    printf("Thread n°%i : je vérouille le verrou\n", args->idThread);

    int attente = 1;

    // Simulation d'un long calcul pour le travail avant RdV
    printf("Thread n°%i : début calcul, duration : %is\n", args->idThread, attente*3);
    calcul(attente);
    printf("Thread n°%i : fin calcul\n", args->idThread);

    // On incrémente le nombre de thread en attente
    // Si on a N thread en attente alors on réveille tout le monde
    if (++(predicat->waitingThread) == args->N) {
        printf("Thread n°%i : dernier thread je réveille tout le monde !\n", args->idThread);
        pthread_cond_broadcast(args->cond);
    }

    // RdV 
    while (predicat->waitingThread != args->N) {
        // Les threads attendent que tout le monde est fini, l'attente libère le verrou
        // Lorsque le thread se réveille, il attend que le verrou se déverouille
        // Une fois le verrou déverouillé il le récupère et le vérouille à son tour
        printf("Thread n°%i : je rompish et je libère le verrou\n", args->idThread);
        pthread_cond_wait(args->cond, args->verrou);
        printf("Thread n°%i : je me réveille et vérouille le verrou\n", args->idThread);
    }

    // On unlock le verrou locké par la fin du wait
    // Ou par le lock pour le dernier thread
    pthread_mutex_unlock(args->verrou);
    printf("Thread n°%i : je libère le verrou\n", args->idThread);

    attente = 2;

    // Simulation d'un long calcul pour le travail avant RdV
    printf("Thread n°%i : début calcul final, duration : %is\n", args->idThread, attente*3);
    calcul(attente);
    printf("Thread n°%i : fin calcul final\n", args->idThread);

    pthread_exit(NULL);
}

int main(int argc, char * argv[]){
  
  if (argc!=2) {
    cout << " argument requis " << endl;
    cout << "./prog nombre_Threads" << endl;
    exit(1);
  }
 
  // initialisations 
  pthread_t threads[atoi(argv[1])];
  struct params tabParams[atoi(argv[1])];

 ...

  srand(atoi(argv[1]));  // initialisation de rand pour la simulation de longs calculs
 
  // cr�ation des threards 
  for (int i = 0; i < atoi(argv[1]); i++){
    tabParams[i].idThread = 0;
    tabParams[i].varPartagee = ; 

    if (pthread_create(&threads[i], NULL, participant, ) != 0){
      perror("erreur creation thread");
      exit(1);
    }
  }

  // attente de la fin des threards. Partie obligatoire 
  for (int i = 0; i < atoi(argv[1]); i++){
  ...
    }
  cout << "thread principal : fin de tous les threads secondaires" << endl;

  // terminer "proprement". 
  pthread_mutex_destroy(&verrou);
  pthread_cond_destroy(&cond);
}
 
