#include <string.h>
#include <stdio.h>//perror
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
//#include <iostream>
#include <pthread.h>


struct paramsFonctionThread {

  int idThread;

  // si d'autres param�tres, les ajouter ici.

};


void * fonctionThread (void * params){

  struct paramsFonctionThread * args = (struct paramsFonctionThread *) params;
  pthread_t actual = pthread_self();
  pthread_exit(NULL);
  // a compl�ter
  //...
}


int main(int argc, char * argv[]){

  if (argc < 2 ){
    printf("utilisation: %s  nombre_threads  \n", argv[0]);
    return 1;
  }     
  
  pthread_t threads[atoi(argv[1])]; 
  
  // cr�ation des threards 
  for (int i = 0; i < atoi(argv[1]); i++){

    // Le passage de param�tre est fortement conseill� (�viter les
    // variables globles).

    //... compl�ter pour initialiser les param�tres
    if (pthread_create(&threads[i], NULL, fonctionThread, NULL) != 0){
      perror("erreur creation thread");
      exit(1);
    }
    printf("Creation du thread reussi ! thread : %d associe au processeur : %i\n", threads, getpid());
    //int res = pthread_join(threads, NULL);
    //pthread_t actual = pthred_self();
  }


// garder cette saisie et modifier le code en temps venu
  char *c; 
  printf("saisir un caractere \n");
  fgets(c, 1, stdin); //pr�cedemment M ? fgetc pour un caract�re ?

 //... compl�ter

  return 0;
 
}
 
