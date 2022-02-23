#include <string.h>
#include <stdio.h>//perror
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "calcul.h"

struct paramsFonctionThread {
  int idThread;
  // si d'autres param�tres, les ajouter ici.
};


void * fonctionThread (void * params){

  //struct paramsFonctionThread * args = (struct paramsFonctionThread *) params;
  srand(time(NULL));
  int *tableau = params;
  pthread_t actual = pthread_self();
  
  printf("Démarage thread : %lu\n", actual);
  printf("---------- Utilisation en cours ----------\n");
  calcul(1);

  int j = rand() % 75;
  printf("----------------------------\n");
  printf("| Case du tableau n°3 : %i |\n", tableau[2]);
  printf("----------------------------\n");
  printf("Modification du tableau : case 3\n");
  tableau[2] = j;
  printf("--- > Case 3 modifiée avec la valeur : %i\n", j);
  // exit(1); ça termine et n'affiche pas la fin des threads
  printf("Fin d'éxécution : fin du thread : %lu\n", actual);

  pthread_exit(NULL);
}


int main(int argc, char * argv[]){

  if (argc < 2 ){
    printf("utilisation: %s  nombre_threads  \n", argv[0]);
    return 1;
  }     
  int t[5] = { 1 , 5 , 45 , 3 , 9 };
  pthread_t threads[atoi(argv[1])]; //tableau qui sert à stocker les identifiants de threads (pthread_t sont les identifiants) 
  
  // création des threards 
  for (int i = 0; i < atoi(argv[1]); i++){
    // Le passage de paramètre est fortement conseill� (�viter les variables globles).
    if (pthread_create(&threads[i], NULL, fonctionThread, t) != 0){ //tous les threads appellent fonctionThread, thread[i] récupère l'identifiant système du thread qui vient d'être créée
    //
      perror("erreur creation thread");
      exit(1);
    }
  }

  
  // garder cette saisie et modifier le code en temps venu
  char c; 
  //printf("saisir un caractere \n");
  c = fgetc(stdin); //sert à poursuivre l'exécutioin
  
  printf("Affichage du tableau final : ");
  printf("(");
  for(int i=0; i<5; i++){
    printf(" %i ", t[i]);
  }
  printf(")\n");

  return 0;
 
}
 
