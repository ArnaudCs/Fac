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

  if (argc != 5){
    printf("utilisation : %s ip_serveur port_serveur port_client port_serveur/client\n", argv[0]);
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

  
   /* Etape 5 : recevoir un message du serveur (voir sujet pour plus de détails) */
   socklen_t servAdr = sizeof(srv);
   /*char bytesSent[1000];
   ssize_t servRes = recv(ds, bytesSent, 1000, 0);

   if (servRes == -1) {
      perror("[CLIENT] Erreur lors de la réception du message du serveur ");
      exit(5);
   }
   printf("[CLIENT] %s", bytesSent);*/
   int size; 
   char* message;
   printf("Attente d'un message\n");
   res = recv(ds, &size, sizeof(int), 0);
   if (res == -1) {
	perror("Erreur réception entier");
   }
   else if (res == 0) {
	printf("Réception impossible, connection close\n");
   }
   else {
	printf("Taille du prochain message %i octets\n", size);
	message = (char*) malloc(size);
	res = recv(ds, message, size, 0);

	if (res == -1) {
		perror("Erreur réception entier");
		free(message);
	}
	else if (res == 0) {
		printf("Réception impossible, connection close\n");
	}

	printf("Nombre d'octet : %i, message reçu : %s\n", res, message);
        printf("Envoi du message\n");
        printf("\n");
        if (send(ds, message+1, size, 0) == -1) {
        	perror("Erreur envoi du message");
   	}
   	free(message);
   }
   
   

   //Vous venez de recevoir un message de type chaine de caracteres. L'etape suivante est de renvoyer ce meme message (et uniquement ce message) à l'expediteur, ensuite de recevoir une nouvelle instruction.

   ////////////////////////////////////////////////////////////////////////////////////////////////////////
   printf("\n");
   printf("############ Démarrage partie SERVEUR ############\n");
   printf("\n");

   int ds2 = socket(PF_INET, SOCK_STREAM, 0);
   if (ds2 == -1) {
	  perror("[SERVEUR] Erreur lors de la création de la socket ");
	  exit(1); // je choisis ici d'arrêter le programme car le reste
	  // dépendent de la réussite de la création de la socket.
   }
   printf("[SERVEUR/CLIENT] Création de la socket réussie.\n");

   struct sockaddr_in ad;
   socklen_t len = sizeof(ad);
   ad.sin_family = AF_INET;      
   ad.sin_addr.s_addr = INADDR_ANY;
   
   // Nommage manuel
   ad.sin_port = ntohs(atoi(argv[4]));

   res = bind(ds2, (struct sockaddr *)&ad, sizeof(ad));
   if (res == -1) {
	  perror("[SERVEUR/CLIENT] Erreur lors du nommage de la socket");
	  exit(1);
   }
   // Récupération de l'adresse et du numéro de port
   if (getsockname(ds2, (struct sockaddr *)&ad, &len) == -1) {
	  perror("[SERVEUR/CLIENT] Erreur lors du nommage automatique de la socket");
	  exit(1);
   }
   printf("[SERVEUR/CLIENT] En cours d'exécution : %s:%d\n", inet_ntoa(ad.sin_addr), ntohs(ad.sin_port));
   
   //passer la socket en mode écoute
   res = listen(ds2, 10);
   if (res == -1){
	 printf("Erreur lors du passage en mode écoute");
	 exit(1);
   }
   else {printf("[SERVEUR/CLIENT] Passage en mode écoute réussie\n");} 
   
   //envoi du port au serveur
   printf("[SERVEUR/CLIENT] Envoie du port au serveur\n");
   short port = atoi(argv[4]); 
   printf("[SERVEUR/CLIENT] Port du serveur-client : %i\n", port);
   printf("\n");
   if (send(ds, &port, sizeof(short), 0) == -1) {
       perror("Erreur envoi du port au serveur");
   }
   printf("[SERVEUR/CLIENT] Envoi du port réussit\n");
   shutdown(ds, SHUT_RDWR);
	  
   struct sockaddr_in sockClient;
   socklen_t lgAdr = sizeof(struct sockaddr_in);
   printf("[SERVEUR/CLIENT] Attente de connexion client\n");
   int dsclient = accept(ds2, (struct sockaddr*)&sockClient, &lgAdr);

   if ( dsclient == -1){
   	perror("Erreur lors de l'acceptation de la demande de connexion\n");
	exit(1);
   }
   printf("[SERVEUR/CLIENT] Connexion réussie avec le client\n");

   for(int i = 0; i<2; i++){
	int size2; 
	printf("[SERVEUR/CLIENT] Attente d'un message\n");
	res = recv(dsclient, &size2, sizeof(int), 0);
	if (res == -1) {
		perror("Erreur réception entier");
	}
	else if (res == 0) {
		printf("Réception impossible, connection close\n");
                exit(1);
	}
	else {
		printf("[SERVEUR/CLIENT] Taille du prochain message %i octets\n", size2);
		char* message = (char*) malloc(size);
		res = recv(dsclient, message, size2, 0);

		if (res == -1) {
			perror("Erreur réception entier");
			free(message);
		}
		else if (res == 0) {
			printf("Réception impossible, connection close\n");
			exit(1);
		}
		printf("[SERVEUR/CLIENT] Nombre d'octet : %i, message reçu : %s\n", res, message);
		free(message);
      }
   }

   printf("\n");
   printf("############ Démarrage partie SERVEUR itératif ############\n");
   printf("\n");

    while (1) {
        struct sockaddr_in clientSockAddr;
        socklen_t lgAdr2;
        char strIp[INET_ADDRSTRLEN];
        int clientSocket = accept(ds2, (struct sockaddr*)&sockClient, &lgAdr2);
        if (clientSocket == -1) {
            perror("Erreur lors de l'acceptation");
            exit(1);
        }

        int pid = fork();

        if (pid == 0) {
            
            printf("[SERVEUR/ITERATIF] Entrée dans le processus fils, numéro : %d\n", getpid());
            printf("[SERVEUR/ITERATIF] Le client connecté est %s:%i.\n", inet_ntoa(clientSockAddr.sin_addr), ntohs(clientSockAddr.sin_port));
            
            int size3; 
            res = recv(clientSocket, &size3, sizeof(int), 0);
            if (res == -1) {
                perror("Erreur réception entier");
            }
            else if (res == 0) {
                printf("Réception impossible, connection close\n");
                        exit(1);
                        close(clientSocket);
            }
            else {
                printf("[SERVEUR/ITERATIF] Taille du prochain message %i octets\n", size3);
                char* message2 = (char*) malloc(size);
                res = recv(clientSocket, message2, size3, 0);

                if (res == -1) {
                    perror("Erreur réception entier");
                    free(message2);
                }
                else if (res == 0) {
                    printf("Réception impossible, connection close\n");
                    exit(1);
                    close(clientSocket);
                }
                printf("[SERVEUR/ITERATIF] Nombre d'octet : %i, message reçu : %s\n", res, message2);
                free(message2);
              }





            res = recv(clientSocket, &size3, sizeof(int), 0);
            if (res == -1) {
                perror("Erreur réception entier");
            }
            else if (res == 0) {
                printf("Réception impossible, connection close\n");
                        exit(1);
                        close(clientSocket);
            }
            else {
                printf("[SERVEUR/ITERATIF] Taille du prochain message %i octets\n", size3);
                char* message2 = (char*) malloc(size);
                res = recv(clientSocket, message2, size3, 0);

                if (res == -1) {
                    perror("Erreur réception entier");
                    free(message2);
                }
                else if (res == 0) {
                    printf("Réception impossible, connection close\n");
                    exit(1);
                    close(clientSocket);
                }
                printf("[SERVEUR/ITERATIF] Nombre d'octet : %i, message reçu : %s\n", res, message2);
                free(message2);
              }

            printf("[SERVEUR/ITERATIF] Fermeture de la connexion.\n");
	        printf("[SERVEUR/ITERATIF] --------------------------------------------------\n");
            printf("\n");
            close(clientSocket);
            break;
        }
        close(clientSocket);
    }
        /*Nombre d'octet : 455, message reçu : Bravo, vous avez atteint la derniere etape. Il reste a modifier votre code pour que la partie serveur de votre programme soit capable de traiter 4 clients, un apres l'autre 
        (serveur iteratif). Quatre clients enverront une demande de connexion. Attention, l'un des clients est malicieux et peut se deconnecter a n'importe quel moment. Ce comportement ne doit pas arreter votre serveur, 
        qui doit pouvoir echanger avec un autre client qui se connecte apres.


   /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
    close(ds);
    close(ds2);
    shutdown(ds2, SHUT_RDWR); //free(msgUser);

    printf("[CLIENT] Sortie.\n");
  return 0;
}