#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "calcul.h"

struct varPartagees { // structure qui regroupe les variables partagées entre les threads.
  int nbZones; //nombres de zones données en paramètre, initialisation dans le main
  int * di; 
  pthread_cond_t cond; // Tableau des conditions deux à deux (pas initialisé)
  pthread_mutex_t lock; //création du mutex (pas initialisé)
};

struct params {  // structure qui regroupe les paramétres d'un thread
  int idThread; // cet entier correspond au numéro de traitement associé à un thread
  struct varPartagees * vPartage; //structure qui contient les paramètres
};

// fonction associée à chaque thread secondaire à créer.
void * traitement (void * p) {

  struct params * args = (struct params *) p; 
  struct  varPartagees * vPartage = args -> vPartage;

  printf("Début traitement n°%i\n",args->idThread); //affichage du thread appellant 

  for(int i = 0; i < vPartage->nbZones; i++){ //pour i inférieur au nombre de zones
    pthread_mutex_lock(&vPartage->lock); //lock du mutex pour que le thread puisse opérer
    
    if(args->idThread != 0){ // le premier traitement n'attent personne
      while(vPartage->di[i] != args->idThread -1){ //tant que le traitement n'est pas fini
        printf("Traitement %i attends la zone %i\n", args->idThread,i); 
        pthread_cond_wait(&vPartage->cond, &vPartage->lock); //mis en attente avec la variable conditionnelle
      }
    }
    pthread_mutex_unlock(&vPartage->lock); //débloquage du thread
    

    // dans cette partie, le traitement de la zone i est à faire en faisant une simulation d'un long calcul (appel a calcul(...)
    printf("Traitement n°%i de la zone %i \n",args->idThread, i);
    calcul(2 + args->idThread); //temps de 2+(id de thread)*3 secondes 
    printf("Fin du traitement n°%i de la zone %i \n",args->idThread, i);
    pthread_mutex_lock(&vPartage->lock); //lock du mutex pour modifier la ressource partagée ici di
    vPartage->di[i] = args->idThread;

    // a la fin du traitement d'une zone, le signaler pour qu'un thread en attente se réveille. 
    pthread_cond_broadcast(&vPartage->cond); //fin de la condition et réveil des threads
    pthread_mutex_unlock(&vPartage->lock);  //débloquage du mutex
  }
  pthread_exit(NULL);
}

int main(int argc, char * argv[]){
  
  if (argc!=3) {
    printf("Utilisation: %s nombre_traitements nombre_zones\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // initialisations 
  pthread_t threads[atoi(argv[1])]; //création du tableau de thread
  struct params tabParams[atoi(argv[1])]; //tableau de paramètres
  struct varPartagees vPartage;
  vPartage.nbZones =  atoi(argv[2]); //nombre de zones en argument initialisation des var partagées
  vPartage.di = malloc(atoi(argv[2])*sizeof(int)); 

  
  int err; //initialisation du mutex et de la cond
  if ((err = pthread_mutex_init(&vPartage.lock, NULL)) != 0) {
      printf("Erreur : %s\n", strerror(err));
      exit(EXIT_FAILURE);
  }
  if ((err = pthread_cond_init(&(vPartage.cond), NULL)) != 0) {
      printf("Erreur : %s\n", strerror(err));
      exit(EXIT_FAILURE);
  }
  
  srand(atoi(argv[1]));  // initialisation de rand pour la simulation de longs calculs
 
  // création des threads 
  for (int i = 0; i < atoi(argv[1]); i++){
    tabParams[i].idThread = i;
    tabParams[i].vPartage = &vPartage; 
    if (pthread_create(&threads[i], NULL, traitement, &tabParams[i]) != 0){ //appel de la fonction de création avec tableau de paramètres
      perror("erreur creation thread");
      exit(1);
    }
  }

  // attente de la fin des  threards. Partie obligatoire 
  for (int i = 0; i < atoi(argv[1]); i++){ //attente de la fin de chaque threads
    pthread_join(threads[i], NULL);
  }
  printf("Thread principal : fin de tous les threads secondaires.\n");

  // libérer les ressources avant terminaison 
  pthread_cond_destroy(&vPartage.cond);
  pthread_mutex_destroy(&vPartage.lock);
  free(vPartage.di); //libération des mallocs
  return 0;
}
