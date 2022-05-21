#include <string.h>
#include <stdio.h>//perror
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "calcul.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct paramsFonctionThread {
  int idThread;
  int tab[5];
};

void * fonctionThread (void * params){
  struct paramsFonctionThread * args = (struct paramsFonctionThread *) params;
  srand(time(NULL));
  pthread_t actual = pthread_self();
  pthread_mutex_lock(&mutex);
  printf("Démarage thread : %i\n", args->idThread); //actual
  printf("---------- Utilisation en cours ----------\n");
  calcul(1);
  int j = rand() % 75;
  printf("----------------------------\n");
  printf("| Case du tableau n°3 : %i |\n", args->tab[2]);
  printf("----------------------------\n");
  printf("Modification du tableau : case 3\n");
  args->tab[2] = j;
  printf("--- > Case 3 modifiée avec la valeur : %i\n", j);
  printf("Fin d'éxécution : fin du thread : %i\n", args->idThread); //actual
  printf("\n");
  args->idThread += 1;
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

int main(int argc, char * argv[]){

  if (argc < 2 ){
    printf("utilisation: %s  nombre_threads  \n", argv[0]);
    return 1;
  }     
  pthread_t threads[atoi(argv[1])]; //tableau qui sert à stocker les identifiants de threads (pthread_t sont les identifiants) 
  struct paramsFonctionThread *args = (struct paramFonctionThread *)malloc(sizeof(struct paramsFonctionThread));
  // création des threards 
  for(int i = 0; i < 5; i++){
      args->tab[i] = rand() % 10; 
  }
  args->idThread = 0;
  for (int i = 0; i < atoi(argv[1]); i++){
    if (pthread_create(&threads[i], NULL, fonctionThread, (void *) args) != 0){ 
      perror("erreur creation thread");
      exit(1);
    }
  }

  for(int j = 0; j < atoi(argv[1]); j++){ //attente de la fin de tous les threads
        pthread_join(threads[j], NULL);
  }
  pthread_mutex_destroy(&mutex);
  //affichage du tableau final
  printf("Affichage du tableau final : ");
  printf("(");
  for(int i=0; i<5; i++){
    printf(" %i ", args->tab[i]);
  }
  printf(")\n");

  return 0;
 
}