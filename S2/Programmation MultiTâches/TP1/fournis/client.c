#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>

/* Programme client */

int main(int argc, char *argv[]) {

  /* je passe en paramètre l'adresse de la socket du serveur (IP et
     numéro de port) et un numéro de port à donner à la socket créée plus loin.*/

  /* Je teste le passage de parametres. Le nombre et la nature des
     paramètres sont à adapter en fonction des besoins. Sans ces
     paramètres, l'exécution doit être arrétée, autrement, elle
     aboutira à des erreurs.*/
  if (argc != 4){
    printf("utilisation : %s ip_serveur port_serveur port_client\n", argv[0]);
    exit(1);
  }

  /* Etape 1 : créer une socket */   
  int ds = socket(PF_INET, SOCK_DGRAM, 0);

  /* /!\ : Il est indispensable de tester les valeurs de retour de
     toutes les fonctions et agir en fonction des valeurs
     possibles. Voici un exemple */
  if (ds == -1){
    perror("Client : pb creation socket :");
    exit(1); 
  }
    // je choisis ici d'arrêter le programme car le reste
    // dépendent de la réussite de la création de la socket.
  
  
  /* J'ajoute des traces pour comprendre l'exécution et savoir
     localiser des éventuelles erreurs */
  printf("Client : creation de la socket réussie \n");
  
  // Je peux tester l'exécution de cette étape avant de passer à la
  // suite. Faire de même pour la suite : n'attendez pas de tout faire
  // avant de tester.
  
  /* Etape 2 : Nommer la socket du client */
  printf("Client : creation de la socket réussie \n");
  struct sockaddr_in client;
  ad.sin_family = AF_INET;
  ad.sin_addr.s_addr = INADDR_ANY;
  ad.sin_port = htons((short) atoi(argv[3]));

  int res = bind(ds, (struct sockaddr*)&ad, sizeof(client));

  //cas d'erreur
  
  if (res == -1) {
    perror("Client : pb nommage socket :");
    close(ds);
    exit(1) 
  }

  printf("bind fait, adresse + port : %i:%i", client.sin_addr.s_addr, client.sin_port);
  getsockname(ds, (struct sockaddr*) &ad, &taille);
  printf("Nommage de la socket cliente réussie %i:%i\n", ad.sin_addr.s_addr, ad.sin_port);

  /* Etape 3 : Désigner la socket du serveur */

  struct sockaddr_in serveur;
  ad.sin_family = AF_INET;
    ad.sin_port = htons((short) atoi(argv[2]));
  ad.sin_addr.s_addr = inet_addr(argv[1]);
  socklen_t lad_serveur = sizeof(serveur);

  /* Etape 4 : envoyer un message au serveur  (voir sujet pour plus de détails)*/
  char* message = "test côté client";
  int conf = sendto(ds, (const char *) message, strlen(message)+1, 0, (const struct sockaddr*)&serveur, lad_serveur);

  if (res == -1) {
    perror("Client : pb envoie message :");
    close(ds);
    exit(1)
  }
  
  /* Etape 5 : recevoir un message du serveur (voir sujet pour plus de détails)*/
  printf("Attente du message en provenance du serveur...\n");
  int recu;
  int recu_s = recvfrom(ds, &recu, sizeof(int), 0, (struct sockaddr *) &serveur, &lad_serveur);

  if (res == -1){
    printf("Erreur lors de la réception");
    close(ds);
    exit(1)
  }
  
  printf("Reponse du serveur : %i", recu);
  
  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
  int close (ds);

  if (res == -1){
    perror("Client : problème lors de la fermeture de la socket");
  }
  
  printf("Client : je termine\n");
  return 0;
}
