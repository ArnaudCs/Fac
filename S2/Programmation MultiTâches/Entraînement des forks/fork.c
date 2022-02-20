#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>//perror
#include <unistd.h>

// une base pour commencer à mettre en place des arboressances de processus avec fork()

int  main (int argc, char *argv[]){

  if (argc != 2) {
    printf ("Utilisation : %s nombre_de_processus_fils\n", argv[0]);
    exit(1);
  }

  int nbProc = atoi(argv[1]);

  int parent = getpid();

  printf("------------------------------------------\n");
  printf("| Avant fork : pid parent racine : %d |\n", parent);
  printf("------------------------------------------\n");
  
  int i = 0;
  while(i < nbProc){
    int resFork = fork();
    if (resFork == -1) {
    perror("Erreur fork : "); 
    exit(1);
    }

    if (parent == getpid()){ // je suis dans le processus racine
       //printf("[PERE] Après fork : je suis le processus : %d et je commence mon travail\n", parent);
       //printf("\n");
       sleep (5); // s'endormir pendant 5 secondes      
    }
    
    if (parent != getpid()){
      printf("[FILS n°%i] Après fork : je suis le processus : %d et je commence mon travail \n", i, getpid());
      printf("\n");
      sleep(3);
      printf("[+ TRAVAIL TERMINÉ +] Je suis le processus : %d et j'ai terminé mon travail\n", getpid());
      printf("\n");
      exit(0);
    }
    i++;
  }
   
  while(wait(0)!=-1);  // cette instruction permet d'attendre la fin de l'execution de tous mes fils.
  printf("[FINAL] Je suis le processus : %d, mon(mes) fils s'est(se sont) terminé(s), je peux terminer à mon tour\n", getpid()); 
  
  return 0;
}







