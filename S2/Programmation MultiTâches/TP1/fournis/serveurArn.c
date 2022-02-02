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
  struct sockaddr_in ad;
  ad.sin_family = AF_INET;
  ad.sin_addr.s_addr = INADDR_ANY;
  ad.sin_port = htons((short) atoi(argv[1]));

  int res = bind(ds, (struct sockaddr*) &ad, sizeof(ad));

  if (res  == -1){
     perror("erreur bind \n");
     close(ds);
     exit(1);
  }

  printf("bind fait, adresse + port %i:%i \n",ad.sin_addr.s_addr,ad.sin_port); 

 
  /* Etape 4 : recevoir un message du client (voir sujet pour plus de détails)*/

  struct sockaddr_in sockClient;
  socklen_t lgAdr;
  ad.sin_family = AF_INET;
  char str[INET_ADDRSTRLEN];
  char *msgRenvoi = "Merci pour ton message";
  char msg[1000];
  int sizemsg = 1000;
  res = -1;
  while(1){
	res = recvfrom(ds, &msg, sizemsg, 0, (struct sockaddr *) &sockClient, &lgAdr);
        inet_ntop(AF_INET, &sockClient.sin_addr, str, INET_ADDRSTRLEN);
	printf("Message de longueur %i reçu de l'adresse %s\n", res, str);
	char type = msg[0];
	
	char len[100];
        sprintf(len, "Taille du message reçu par le serveur : %zu\n", strlen(msg));
        if (sendto(ds, len, strlen(len) + 1, 0, (const struct sockaddr*)&sockClient, lgAdr) == -1) {
            perror("[SERVEUR] Erreur lors du retour au client ");
            exit(5);
        }
  }
  
  /* Etape 5 : envoyer un message au serveur (voir sujet pour plus de détails)*/

  char* message = "test serveur-client (cote serveur)";
  int conf = sendto(ds, (const char *) message, strlen(message)+1, 0, (const struct sockaddr*)&sockClient, lgAdr);

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
