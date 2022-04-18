
/*
 Programme ronde avec un thread bus et des threads visiteurs. 
 Les zones � compl�ter sont indiqu�es en commentaires.
 
 Les traces fournies sont suffisantes.
 
 Vous avez � votre disposition test/zoo qui est un ex�cutable vous illustrant le comportement attendu.
 
*/

#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"

struct varPartagees {
  // structure regroupant toutes le varables partag�es entre threads
  pthread_mutex_t lock; //création du mutex
  pthread_cond_t buscomplet; //création de la variable conditionnelle 
  pthread_cond_t descente; //création de la variable conditionnelle 
  int nbPlaces; // nombre total de places dans le bus
  int cptPlace;
  // ... zone � compl�ter 
};

//FAIRE DES LOCK UNLOCK avant des IF ou des WHILE permet de voir si la valeur que je vais comparer n'est pas en train d'être modifiée par un autre processus

struct params {
  // structure pour regrouper les param�tres d'un thread. 
  int idThread; // un identifiant de visiteur, de 1 � N (N le nombre total de visiteurs)
  struct varPartagees * varP;
};

// pour le thread bus
void * bus (void * p) {
  struct params * args = (struct params *) p;
  struct varPartagees * varP  = args -> varP;
  while(1){
   afficher('b', "embarquement immediat, soyez les bienvenus!", 0);
   
   // ... zone � compl�ter pour lancer l'embarquement
   if(varP->cptPlace != 0){
     pthread_cond_wait(&varP->buscomplet, &varP->lock);
     afficher('b', "attente que le bus soit complet", 0);
   }
   else if(varP->cptPlace == varP->nbPlaces){ //si tous les occupants sont montés
     afficher('b', "Tous les occupants sont dans le bus", 0);
     pthread_cond_broadcast(&varP->buscomplet);
   }
  pthread_mutex_unlock(&varP->lock);
   /* ... zone a compl�ter pour attendre que le bus soit plein
    cette zone doit inclure la ligne :
    
   */
   afficher('b',"bus complet, la visite commence !", 0); 

   visite(1); // vous pouvez changer la valeur du param�tre (voir .h)
   
   // ... zone � compl�ter pour ordonner la descente du bus
   afficher('b',"visite terminee, tout le monde descend !", 0);
   pthread_mutex_lock(&varP->lock);
   varP->cptPlace -= 1; //on décrémente le nombre de places occupées
   if(varP->cptPlace != 0){
     pthread_cond_wait(&varP->descente, &varP->lock);
     afficher('b', "attente que tout le monde descende", 0);
   }
   else if(varP->cptPlace == 0){ //si tous les occupants sont decendus
     printf("Tous les occupants sont bien descendus - Nombre de place occupées : %i\n", varP->cptPlace); 
     pthread_cond_broadcast(&varP->descente);
   }
  pthread_mutex_unlock(&varP->lock);
   /* ... zone a compl�ter pour attendre que le bus soit vide
    cette zone doit inclure la ligne :
    
      afficher('b', "attente que tout le monde descende", 0);
   */
  }
  pthread_exit(NULL); 
}


// pour le thread visiteur
void * visiteur (void * p) {
  struct params * args = (struct params *) p;
  struct  varPartagees * varP = args -> varP;

   afficher('v', "je demande a monter", args -> idThread);
   
   /* 
   ... zone a compl�ter pour mettre en place la mont�e.
   Elle doit inclure la ligne :
   afficher('v', "pas de place, j'attends", args -> idThread);
   */
   
   // simulation mont�e du visiteurpthread_mutex_lock(&varP->lock);
   pthread_mutex_lock(&varP->lock);
   if(varP->cptPlace < varP->nbPlaces){
     pthread_mutex_unlock(&varP->lock);
     pthread_mutex_lock(&varP->lock);
     varP->cptPlace += 1;
     afficher('v', "je monte ...", args -> idThread);
     afficher('v', "Nombre de places occupées %i", varP->cptPlace);
     pthread_mutex_unlock(&varP->lock);
   }else if(varP->cptPlace == varP->nbPlaces){
     pthread_cond_wait(&varP->buscomplet, &varP->lock);
   }

   if(varP->cptPlace == 0){ //si c'est = à 0 c'est que tout le monde est descendu et c'est donc une nouvelle ronde
    pthread_cond_broadcast(&varP->descente);
    pthread_mutex_lock(&varP->lock);
    varP->cptPlace += 1; //nouvelle ronde, on monte
   }
   
   monterOuDescendre();
   // .. zone qui pourrait �ventuellement vous �tre utile
   afficher('v', "je suis a bord et bien installe !", args -> idThread);
   visite(1); //fait une visite
   /*
   ... zone � compl�ter pour la mise en place de la descente du passager.
   Elle doit inclure la ligne :
   
    afficher('v', "j'attends la fin de la visite", args -> idThread);
   */
  
   afficher('v', "visite terminee, je descends ...", args -> idThread);
   // .. zone qui pourrait �ventuellement vous �tre utile

   monterOuDescendre();
   // .. zone qui pourrait �ventuellement vous �tre utile
    
   pthread_mutex_lock(&varP->lock);
   varP->cptPlace -= 1;
   afficher('v', "je descends ...", args -> idThread);
   afficher('v', "je suis descendu !", args -> idThread);
    
   if(varP->cptPlace > 0){
     pthread_mutex_unlock(&varP->lock);
     pthread_mutex_lock(&varP->lock);
     pthread_cond_wait(&varP->descente, &varP->lock);
   }else if(varP->cptPlace == 0){
     pthread_cond_broadcast(&varP->descente);
   }   
  pthread_mutex_unlock(&varP->lock);
  pthread_exit(NULL); 
}


int main(int argc, char * argv[]){
  
  if (argc!=3) {
    printf(" argument requis \n %s nombres_places_bus nombre_visiteurs\n", argv[0]);
    exit(1);
  }

  initDefault(atoi(argv[2])); // ne pas modifier cet appel ni le d�placer.
 
 
  // initialisations 
  pthread_t threads[atoi(argv[2])+1];
  struct params tabParams[atoi(argv[2])+1];
 
  struct varPartagees  varP;
  
  varP.nbPlaces = atoi(argv[1]);
  varP.nbPlaces = 0;
  int res;
  if ((res = pthread_mutex_init(&varP.lock, NULL)) != 0) {
      printf("Erreur : %s\n", strerror(res));
      exit(EXIT_FAILURE);
  }
  if ((res = pthread_cond_init(&varP.buscomplet, NULL)) != 0) { 
      printf("Erreur : %s\n", strerror(res));
      exit(EXIT_FAILURE);
  }
  if ((res = pthread_cond_init(&varP.descente, NULL)) != 0) { 
      printf("Erreur : %s\n", strerror(res));
      exit(EXIT_FAILURE);
  }

  
  //... zone � compl�ter pour initialiser les tous les champs de varP

 
  // cr�ation des threads
  tabParams[0].idThread = 0;
  tabParams[0].varP = &varP; 
  if (pthread_create(&(threads[0]), NULL, bus, &(tabParams[0])) != 0){
      perror("erreur creation thread bus");
      exit(1);
  }  
  for (int i = 1; i < atoi(argv[2])+1; i++){
    tabParams[i].idThread = i;
    tabParams[i].varP = &varP; 
    if (pthread_create(&threads[i], NULL, visiteur, &(tabParams[i])) != 0){
      perror("erreur creation thread visiteur");
      exit(1);
    }
  }
  

  // attente de la fin des  threads. 
  for (int i = 0; i < atoi(argv[2])+1; i++)
    if (pthread_join(threads[i], NULL) != 0){
      perror("erreur attente thread");
      exit(1);
    }
    
  // ... zone � compl�ter pour terminer proprement votre programme.

  pthread_cond_destroy(&varP.buscomplet);
  pthread_cond_destroy(&varP.descente);
  pthread_mutex_destroy(&varP.lock);
 
  return 0;
}
 
