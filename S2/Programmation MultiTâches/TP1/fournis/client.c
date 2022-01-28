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

  char* ip_serveur = argv[1];
  int port_serveur = argv[2];
  int port = atoi(argv[3]);
  
  // Je peux tester l'exécution de cette étape avant de passer à la
  // suite. Faire de même pour la suite : n'attendez pas de tout faire
  // avant de tester.
  
  /* Etape 2 : Nommer la socket du client */
  printf("Client : creation de la socket réussie \n");
  struct sockaddr_in ad;
  ad.sin_family = AF_INET;
  ad.sin_addr.s_addr = INADDR_ANY;
  ad.sin_port = htons((short) port);
  int res = bind(ds, (struct sockaddr*)&ad, sizeof(ad));
  //cas d'erreur
  if (res == -1) {
    perror("Client : pb nommage socket :");
    exit(1); 
  }
  
  int taille = sizeof(ad);
  getsockname(ds, (struct sockaddr*) &ad, &taille);
  printf("%i\n", ntohs((short) ad.sin_port));
  
  printf("Nommage de la socket cliente réussie %i:%i\n", ad.sin_addr.s_addr, ad.sin_port);

  /* Etape 3 : Désigner la socket du serveur */

  struct sockaddr_in ad_srv;
  ad.sin_family = AF_INET;
  ad.sin_addr.s_addr = inet_addr(argv[1]);
  ad.sin_port = htons((short) atoi(argv[2]));
  printf("Désignation de la socket coté serveur réussie %i:%i\n", ad_srv.sin_addr.s_addr, ad_srv.sin_port);


  /* Etape 4 : envoyer un message au serveur  (voir sujet pour plus de détails)*/
  char message [200];
  printf("Envoyez un message au serveur (200 charactères)\n");
  fgets(message, 200);

  size_t len_msg = strlen(message);
  socklen_t len_adr = sizeof(struct sockaddr_in);
  
  res = sendto(ds, message, len_msg + 1, 0, (struct sockaddr*) &ad_srv, len_adr);

  if (res == -1) {
    perror("Client : pb envoie message :");
    exit(1); 
  }
  else if(res<=len_msg) {
    printf("Client : seul %i octet ont été envoyés");
  }
  
  /* Etape 5 : recevoir un message du serveur (voir sujet pour plus de détails)*/
  printf("Attente du message en provenance du serveur...\n");

  struct sockaddr_in sock_srv;
  res = recvfrom(ds, message, sizeof(message), 0, (struct sockaddr*) &sock_srv, &len_adr);

  if (res == -1){
    printf("Erreur lors de la réception");
  }
  else {
    printf("Client : message reçu du serveur \n");
    printf("\t\t%s", message);
  }
  
  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
  int close (ds);

  if (res == -1){
    perror("Client : problème lors de la fermeture de la socket");
  }
  
  printf("Client : je termine\n");
  return 0;
}
