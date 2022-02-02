#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>

/* Programme serveur */

int main(int argc, char *argv[]) {

  /* Je passe en paramètre le numéro de port qui sera donné à la socket créée plus loin.*/

  /* Je teste le passage de parametres. Le nombre et la nature des
     paramètres sont à adapter en fonction des besoins. Sans ces
     paramètres, l'exécution doit être arrétée, autrement, elle
     aboutira à des erreurs.*/
  if (argc != 2){
    printf("utilisation : %s port_serveur\n", argv[0]);
    exit(1);
  }

  /* Etape 1 : créer une socket */   
  int ds = socket(PF_INET, SOCK_DGRAM, 0);

  /* /!\ : Il est indispensable de tester les valeurs de retour de
     toutes les fonctions et agir en fonction des valeurs
     possibles. Voici un exemple */
  if (ds == -1){
    perror("Serveur : pb creation socket :");
    exit(1); // je choisis ici d'arrêter le programme car le reste
	     // dépendent de la réussite de la création de la socket.
  }
  
  /* J'ajoute des traces pour comprendre l'exécution et savoir
     localiser des éventuelles erreurs */
  printf("Serveur : creation de la socket réussie \n");
  
  // Je peux tester l'exécution de cette étape avant de passer à la
  // suite. Faire de même pour la suite : n'attendez pas de tout faire
  // avant de tester.
  
  /* Etape 2 : Nommer la socket du seveur */
  struct sockaddr_in serveur;
  serveur.sin_family = AF_INET;
  serveur.sin_addr.s_addr = INADDR_ANY;
  serveur.sin_port = htons((short) atoi(argv[1]));

  int res = bind(ds, (struct sockaddr*) &serveur, sizeof(serveur));

  if (res  == -1){
     perror("erreur bind \n");
     close(ds);
     exit(1);
  }

  printf("bind fait, adresse + port %i:%i \n",serveur.sin_addr.s_addr,serveur.sin_port); 


  struct sockaddr_in client;
  serveur.sin_family = AF_INET;
  serveur.sin_addr.s_addr = INADDR_ANY;
  serveur.sin_port = htons((short) atoi(argv[]));
  socklen_t lad_client = sizeof(client);
 
  /* Etape 4 : recevoir un message du client (voir sujet pour plus de détails)*/
  int recu;
  int recu_s = recvfrom(ds, &recu, sizeof(int), 0, (struct sockaddr *) &client, &lad_client);

  if (recu_s == -1){
      perror("pb recvfrom \n");
      close(ds);
      exit(1);
  }

  printf("reponse du client : %i \n", recu);
  
  /* Etape 5 : envoyer un message au serveur (voir sujet pour plus de détails)*/

  char* message = "test serveur-client (cote serveur)";
  int conf = sendto(ds, (const char *) message, strlen(message)+1, 0, (const struct sockaddr*)&client, lad_client);

  if (conf == -1){
      perror("message non envoyé \n");
      close(ds);
      exit(1);
  }
  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
  close(ds);
  
  printf("Serveur : je termine\n");
  return 0;
}
