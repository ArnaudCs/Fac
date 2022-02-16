#include <netinet/in.h>
#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

/* Programme serveur */

int recvTCP(int sock, void* msg, int sizeMsg) {
	int remaining = sizeMsg;

	while (remaining > 0) {
		printf("Receive remaining : %i\n", remaining);
		int res = recv(sock, msg + sizeMsg - remaining, remaining, 0);
		if (res <= 0) {
			return res;
		}
		remaining -= res;
	}
	return 1;
}


int main(int argc, char *argv[]) {
   /* Je passe en paramètre le numéro de port qui sera donné à la socket créée plus loin.*/

   /* Je teste le passage de parametres. Le nombre et la nature des
	  paramètres sont à adapter en fonction des besoins. Sans ces
	  paramètres, l'exécution doit être arrétée, autrement, elle
	  aboutira à des erreurs.*/
   if (argc != 2){
	  printf("Utilisation : %s [port_serveur]\n", argv[0]);
	  exit(1);
   }

   /* Etape 1 : créer une socket */   
   int ds = socket(PF_INET, SOCK_STREAM, 0);

   /* /!\ : Il est indispensable de tester les valeurs de retour de
	  toutes les fonctions et agir en fonction des valeurs
	  possibles. Voici un exemple */
   if (ds == -1) {
	  perror("[SERVEUR] Erreur lors de la création de la socket ");
	  exit(1); // je choisis ici d'arrêter le programme car le reste
	  // dépendent de la réussite de la création de la socket.
   }

   /* J'ajoute des traces pour comprendre l'exécution et savoir
	  localiser des éventuelles erreurs */
   printf("[SERVEUR] Création de la socket réussie.\n");

   // Je peux tester l'exécution de cette étape avant de passer à la
   // suite. Faire de même pour la suite : n'attendez pas de tout faire
   // avant de tester.

   /* Etape 2 : Nommer la socket du serveur */
   struct sockaddr_in ad;
   socklen_t len = sizeof(ad);
   ad.sin_family = AF_INET;      
   ad.sin_addr.s_addr = INADDR_ANY;
   
   //passer la socket en mode écoute

   // Nommage manuel
   ad.sin_port = ntohs(atoi(argv[1]));

   int res = bind(ds, (struct sockaddr *)&ad, sizeof(ad));
   if (res == -1) {
	  perror("[SERVEUR] Erreur lors du nommage de la socket ");
	  exit(1);
   }

   // Récupération de l'adresse et du numéro de port
   if (getsockname(ds, (struct sockaddr *)&ad, &len) == -1) {
	  perror("[SERVEUR] Erreur lors du nommage automatique de la socket ");
	  exit(1);
   }

   printf("[SERVEUR] En cours d'exécution : %s:%d\n", inet_ntoa(ad.sin_addr), ntohs(ad.sin_port));
   
   //passer la socket en mode écoute
   res = listen(ds, 10);
   if (res == -1){
	 printf("Erreur lors du passage en mode écoute");
	 exit(1);
   }
   else {printf("Passage en mode écoute réussie\n");} 
	  
   struct sockaddr_in sockClient;
   socklen_t lgAdr = sizeof(struct sockaddr_in);
   int dsclient = accept(ds, (struct sockaddr*)&sockClient, &lgAdr);

   while (1) {
	  //int dsclient = accept(ds, (struct sockaddr*)&sockClient, &lgAdr);


	  /* Etape 4 : recevoir un message du client (voir sujet pour plus de détails)*/
	  /*int msgSize = 4000;
	  char msg[4000];
	  ssize_t msgres = recv(dsclient, msg, msgSize, 0);
	  printf(" --- Connexion réussie : %s:%i ---\n", inet_ntoa(sockClient.sin_addr), ntohs(sockClient.sin_port));*/
	  /*char message[4000]; printf("[Exo 2-1] : Appuyer sur entrée pour mettre le serveur en attente d'un message");   // Pour bloquer l'attente
	  fgets(message, 1500, stdin);*/

		/*int size;
		printf("Attente d'un message\n");

		res = recv(dsclient, &size, sizeof(int), 0);

		if (res == -1) {
		perror("Erreur réception entier");
		}
		else if (res == 0) {
		printf("Réception impossible, connection close\n");
		}
		else {
		printf("Taille du prochain message %i octets\n", size);
		char* message = (char*) malloc(size);
		res = recv(dsclient, message, size, 0);
			if (res == -1) {
				perror("Erreur réception entier");
				free(message);
			}
			else if (res == 0) {
				printf("Réception impossible, connection close\n");
			}

			printf("Nombre d'octet : %i, message reçu : %s\n", res, message);
			free(message);
		}*/

			int size; 
			printf("Attente d'un message\n");

			int res = recv(dsclient, &size, sizeof(int), 0);

			if (res == -1) {
				perror("Erreur réception entier");
			}
			else if (res == 0) {
				printf("Réception impossible, connection close\n");
			}
			else {
				printf("Taille du prochain message %i octets\n", size);
				char* message = (char*) malloc(size);
				res = recv(dsclient, message, size, 0);

				if (res == -1) {
					perror("Erreur réception entier");
					free(message);
				}
				else if (res == 0) {
					printf("Réception impossible, connection close\n");
				}

				printf("Nombre d'octet : %i, message reçu : %s\n", res, message);
				free(message);
			}

	//plusieurs messages avec recv
	/*int size;
	printf("[Exo 2-3] : Attente des messages clients\n");

	int i = 1;
	while (1) {

		//plusieurs messages
		int res = recv(dsclient, &size, sizeof(int), 0);

		if (res == -1) {
			perror("[Exo 2-3] : erreur réception entier");
			exit(1);
		}
		else if (res == 0) {
			printf("[Exo 2-3] : réception impossible, connection close\n");
			exit(1);
		}
		else {
			printf("[Exo 2-3] : Message %i, taille %i octets\n", i, size);
			char* message = (char*) malloc(size);
			int octets_restants = size;

			int j = 0;
			while (octets_restants > 0) {
				res = recv(dsclient, message + (size - octets_restants), octets_restants, 0);
				if (res == -1) {
					perror("[Exo 2-3] : erreur réception entier");
					free(message);
					exit(1);
				}
				else if (res == 0) {
					printf("[Exo 2-3] : réception impossible, connection close\n");
					exit(1);
				}

				octets_restants -= res;
				j++;
			}

			printf("[Exo 2-3] : Message reçu après %i réception(s)\n", j);
		}
		i++;*/

	/*int size;
	printf("[Exo 2-3] : Attente des messages clients\n");

	int i = 1;
	while (1) {
		//plusieurs messages
		int res = recv(dsclient, &size, sizeof(int), 0);

		if (res == -1) {
			perror("[Exo 2-3] : erreur réception entier");
			exit(1);
		}
		else if (res == 0) {
			printf("[Exo 2-3] : réception impossible, connection close\n");
			exit(1);
		}
		else {
			printf("[Exo 2-3] : Message %i, taille %i octets\n", i, size);
			char* message = (char*) malloc(size);
			int octets_restants = size;

		int j = 0;
			while (octets_restants > 0) {
				res = recv(dsclient, message + (size - octets_restants), octets_restants, 0);
				if (res == -1) {
					perror("[Exo 2-3] : erreur réception entier");
					free(message);
					exit(1);
				}
				else if (res == 0) {
					printf("[Exo 2-3] : réception impossible, connection close\n");
					exit(1);
				}
				octets_restants -= res;
				j++;
			}
			printf("[Exo 2-3] : Message reçu après %i réception(s)\n", j);
		}
		i++;*/

		/*printf("[Exo 2-4] : Attente du long message client\n");

		int res = recv(dsclient, &size, sizeof(int), 0);

		if (res == -1) {
		perror("[Exo 2-4] : erreur réception entier");
		exit(1);
		}
		else if (res == 0) {
		printf("[Exo 2-4] : réception impossible, connection close\n");
		exit(1);
		}
		else {
		printf("[Exo 2-4] : taille du long message : %i octets\n", size);
		char* message = (char*) malloc(size);
		int octets_restants = size;

		int j = 0;
		while (octets_restants > 0) {
			res = recv(dsclient, message + (size - octets_restants), octets_restants, 0);
			if (res == -1) {
				perror("[Exo 2-4] : erreur réception entier");
				free(message);
				exit(1);
			}
			else if (res == 0) {
				printf("[Exo 2-4] : réception impossible, connection close\n");
				exit(1);
			}

			octets_restants -= res;
			j++;
		}

		printf("[Exo 2-4] : Message reconstruit après %i réception(s)\n", j);
		}
	}*/
	  

	  /*printf("------------------------------------------------\n");
	  printf("[SERVEUR] Message reçu : %s\n", msg);
	  printf("Adresse du client : %s:%i\n", inet_ntoa(sockClient.sin_addr), ntohs(sockClient.sin_port));
	  printf("------------------------------------------------\n");*/

	  /* Etape 5 : envoyer un message au serveur (voir sujet pour plus de détails)*/
	  char len[400];
	  sprintf(len, "Taille du message reçu par le serveur : %i\n", size);
	  if (send(dsclient, len, strlen(len) + 1, 0) == -1) {
		  perror("[SERVEUR] Erreur lors du retour au client ");
		  exit(1);
	  }
	}

   /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
   
   // On pourrait aussi faire close() mais shutdown() est plus sécurisée et plus pratique à manipuler.
   shutdown(ds, SHUT_RDWR);

   printf("[SERVEUR] Sortie.\n");
   return 0;
}
