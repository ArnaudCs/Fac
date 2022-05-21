/*
  Programme bus � compl�ter. Les zones � compl�ter sont indiqu�es et il n'est pas n�cessaire d'ajouter de nouvelles traces d'ex�cution.
  Vous devez expliquer en commentaires : le sens donn� au messages echang�s et aux �tiquettes.
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include "simulation.h"
#define MSGSZ 128


typedef struct msgbuf{
    long mtype;
    char mtext[MSGSZ];
} message_buf;


int main(int argc, char * argv[]){
    message_buf rbuf;
    size_t len;

  if (argc!=4) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s nombre-places fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
    exit(0);
  }
	  
  int cle=ftok(argv[2], atoi(argv[3]));

  int idFile = msgget(cle, 0666);

  // j'utilise msgget de sorte a s'assurer que la file existe.
  if (idFile==-1){
    perror("erreur  msgget");
    exit(-1);
  }

  printf("Bus : Id File msg : %d \n", idFile);
  
  int nbPlaces = atoi(argv[1]);
  
  // ... zone � utiliser si n�cessaire
  
  while(1){
     // les traces d'ex�cution sont � garder inchang�es.
    printf("Bus : embarquement immediat, soyez les bienvenus! \n");
    
    /* ... zone � compl�ter pour mettre en place l'embarquement
    Elle doit inclure la ligne suivante :
    
      printf("Bus : attente que le bus soit complet \n");
    */
    while(nbPlaces != 0) {
        // Envoie de la donnée au processus         
        if(msgrcv(idFile, &rbuf, MSGSZ, 1, 0) < 0){
          perror("msgrcv");
          if (msgctl(idFile, IPC_RMID, NULL) == -1) {
                perror("erreur suppression file de message :");
            }
          printf("suppression file et sortie");
          exit(EXIT_FAILURE);
        }
        nbPlaces += 1;
        printf("Un passager est monté...\n");
        // Copie dans la variable locale de message
    }
    if(nbPlaces == 0){
      printf("Bus : bus complet, la visite commence !\n");

    }
    visite(2);  // simulation de la visite. voir .h pour le param�tre
    
    printf("Bus : visite terminee, tout le monde descend !\n"); 
    // ... zone � utiliser pour d�clancher la descente du bus
    
    printf("Bus : attendre que le bus soit vide\n");
    if (msgctl(idFile, IPC_RMID, NULL) == -1) {
        perror("erreur suppression file de message :");
    }
    printf("Bus vide ... file supprimée");
    // ... zone � utiliser pour attendre que le bus soit vide
  }
  return 0;
}

