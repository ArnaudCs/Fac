#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>

/* Programme client */

int sendTCP(int sock, void* msg, int sizeMsg) {
    int remaining = sizeMsg;
    while (remaining > 0) {
        printf("Send remaining : %i\n", remaining);
        int res = send(sock, msg + sizeMsg - remaining, remaining, 0);
        if (res <= 0) {
            return res;
        }
        remaining -= res;
    }
    return 1;
}

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
  int ds = socket(PF_INET, SOCK_STREAM, 0);

  /* /!\ : Il est indispensable de tester les valeurs de retour de
     toutes les fonctions et agir en fonction des valeurs
     possibles. Voici un exemple */
  if (ds == -1){
    perror("[Client] : pb creation socket :");
    exit(1); // je choisis ici d'arrêter le programme car le reste
	     // dépendent de la réussite de la création de la socket.
  }
  
  /* J'ajoute des traces pour comprendre l'exécution et savoir
     localiser des éventuelles erreurs */
  printf("[Client] : creation de la socket réussie \n");
  
  // Je peux tester l'exécution de cette étape avant de passer à la
  // suite. Faire de même pour la suite : n'attendez pas de tout faire
  // avant de tester.
  
  /* Etape 2 : Nommer la socket du client
   struct sockaddr_in ad;
   socklen_t len = sizeof(ad);
   ad.sin_family = AF_INET;            // IPv4
   ad.sin_addr.s_addr = INADDR_ANY;
   ad.sin_port = htons(atoi(argv[3])); 
   int res = bind(ds, (struct sockaddr *)&ad, len);
   if (res == -1){
      perror("[Client] : pb nommage socket :");
      exit(1);
  }*/
  
  /* Etape 3 : Désigner la socket du serveur */
   struct sockaddr_in srv;
   srv.sin_family = AF_INET;
   srv.sin_addr.s_addr = inet_addr(argv[1]); //91.174.102.81:32768
   srv.sin_port = htons(atoi(argv[2]));
   socklen_t lgA = sizeof(struct sockaddr_in);
   
   //demande de connexion au serveur 
   printf("Demande de connexion\n");
   int res = connect(ds, (struct sockaddr *)&srv, lgA);
   if(res == -1){
   	printf("erreur lors de la demande de connexion");
   }

  /* Etape 4 : envoyer un message au serveur  (voir sujet pour plus de détails)*/
    //double envoi qui fonctionne.
    /*char message[1500];
    printf("Envoyer un message au serveur (1500 caractères max): ");
    fgets(message, 1500, stdin);
    int lenmsg = strlen(message);
    printf("\n");

    if (send(ds, &lenmsg, sizeof(int), 0) == -1) {
        perror("[Exo 2-1] : erreur envoi de la taille du premier message");
    }
    if (send(ds, message, strlen(message), 0) == -1) {
        perror("[Exo 2-1] : erreur envoi du premier message");
    }
    // deuxième message
    if (send(ds, &lenmsg, sizeof(int), 0) == -1) {
        perror("[Exo 2-1] : erreur envoi de la taille du second message");
    }
    if (send(ds, message, strlen(message)+1, 0) == -1) {
        perror("[Exo 2-1] : erreur envoi du second message");
    }*/

    char message[1500];
    printf("[Exo 2-2] : envoyer un message au serveur (1500 caractères max): ");
    fgets(message, 1500, stdin);
    printf("\n");

    int size_message = strlen(message) + 1; 
    if (send(ds, &size_message, sizeof(int), 0) == -1) {
        perror("Erreur envoi premier entier");
    }
    if (send(ds, message, strlen(message) + 1, 0) == -1) {
        perror("Erreur envoi premier message");
    }
    if (send(ds, &size_message, sizeof(int), 0) == -1) {
        perror("Erreur envoi second entier");
    }
    if (send(ds, message, strlen(message) + 1, 0) == -1) {
        perror("Erreur envoi second message");
    }

    // version qui envoi un certain nombre de messages
    /*char message[1500];
    char nbs[4];
    printf("[Exo 2-3] : envoyer un message au serveur (1500 caractères max): ");
    fgets(message, 1500, stdin);
    printf("\n[Exo 2-3] : combien de fois l'envoyer (entre 0 et 999): ");
    fgets(nbs, 4, stdin);
    printf("\n");

    int nb = atoi(nbs);
    int size_message = strlen(message) + 1;

    printf("[Exo 2-3] : taille message envoyé : %i\n", size_message);
    printf("[Exo 2-3] : Envoie de %i fois ce message au serveur\n", nb);

    for (int i = 0; i < nb; ++i) {
        if (sendTCP(ds, &size_message, sizeof(int)) == -1) {
            perror("[Exo 2-3] : erreur envoi entier");
        }
        if (sendTCP(ds, message, size_message) == -1) {
            perror("[Exo 2-3] : erreur envoi message");
        }
    }*/

     
    /*//Etape 4 surcharge du buffer
    char message[1000000];
    printf("[Exo 2-4] : envoyer un message très long au serveur: ");
    fgets(message, 1000000, stdin);
    printf("\n");

    int size_message = strlen(message) + 1;

    printf("[Exo 2-4] : taille message envoyé : %i\n", size_message);

    if (send(ds, &size_message, sizeof(int), 0) == -1) {
        perror("[Exo 2-4] : erreur envoi entier");
    }
    if (send(ds, message, size_message, 0) == -1) {
        perror("[Exo 2-4] : erreur envoi message");
    }*/


  
   /* Etape 5 : recevoir un message du serveur (voir sujet pour plus de détails) */
   socklen_t servAdr = sizeof(srv);
   char bytesSent[1000];
   ssize_t servRes = recv(ds, bytesSent, 1000, 0);

   if (servRes == -1) {
      perror("[CLIENT] Erreur lors de la réception du message du serveur ");
      exit(5);
   }
   printf("[CLIENT] %s", bytesSent);

   /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
   shutdown(ds, SHUT_RDWR); //free(msgUser);

   printf("[CLIENT] Sortie.\n");
  return 0;
}
