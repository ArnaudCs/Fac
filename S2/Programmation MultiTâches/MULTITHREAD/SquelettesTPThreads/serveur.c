#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread>

int sendTCP(int connectedSocket, char *message, size_t messageSize) {
    ssize_t res = send(connectedSocket, &messageSize, sizeof(messageSize), 0);
    if (res == -1 || res == 0) {
        return res;
    }
    size_t totalSent = 0;
    while (totalSent < messageSize) {
        ssize_t res = send(connectedSocket, message + totalSent, messageSize - totalSent, 0);
        if (res == -1 || res == 0) {
            return res;
        }
        totalSent += res;
    }
    return totalSent;
}

int recvTCP(int connectedSocket, char *message, size_t maxMessageSize) {
    size_t messageSize;
    ssize_t res = recv(connectedSocket, &messageSize, sizeof(messageSize), 0);
    if (res == -1 || res == 0) {
        return res;
    }
    size_t totalReceived = 0;
    while (totalReceived < messageSize) {
        ssize_t res = recv(connectedSocket, message + totalReceived, messageSize - totalReceived, 0);
        if (res == -1 || res == 0) {
            return res;
        }
        totalReceived += res;
    }
    return totalReceived;
}

struct paramsFonctionThread {
  int id;
  short sockclientport;
  struct in_addr sockclientaddr;
  // si d'autres param�tres, les ajouter ici.
};

void * fonctionThread (void * params){

  struct paramsFonctionThread * infos = (struct paramsFonctionThread *) params;
  srand(time(NULL));
  pthread_t actual = pthread_self();
  printf("Démarage thread : %lu\n", actual);
  printf("---------- Utilisation en cours ----------\n");
  
            int size; 
            int res2 = recv(infos->, &size, sizeof(int), 0);
            if (res2 == -1) {
                perror("Erreur réception entier");
            }
            else if (res2 == 0) {
                printf("Réception impossible, connection close\n");
                        close(clientSocket);
            }
            else {
                printf("[SERVEUR/ITERATIF] Taille du prochain message %i octets\n", size);
                char* message2 = (char*) malloc(size);
                res2 = recv(clientSocket, message2, size, 0);

                if (res2 == -1) {
                    perror("Erreur réception entier");
                    free(message2);
                }
                else if (res2 == 0) {
                    printf("Réception impossible, connection close\n");
                    close(clientSocket);
                }
                printf("[SERVEUR/ITERATIF] Nombre d'octet : %i, message reçu : %s\n", res2, message2);
                free(message2);
              }
  // exit(1); ça termine et n'affiche pas la fin des threads
  printf("Fin d'éxécution : fin du thread : %lu\n", actual);

  pthread_exit(NULL);
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
   
   i = 0;
   while (1) {
        struct sockaddr_in clientSockAddr;
        socklen_t lgAdr2;
        char strIp[INET_ADDRSTRLEN];
        int clientSocket = accept(ds, (struct sockaddr*)&sockClient, &lgAdr2);
        if (clientSocket == -1) {
            perror("Erreur lors de l'acceptation");
            exit(1);
        }
        /*int pid = fork();
        if (pid == 0) {
            
            printf("[SERVEUR/ITERATIF] Entrée dans le processus fils, numéro : %d\n", getpid());
            printf("[SERVEUR/ITERATIF] Le client connecté est %s:%i.\n", inet_ntoa(clientSockAddr.sin_addr), ntohs(clientSockAddr.sin_port));
            
            int size; 
            res = recv(clientSocket, &size, sizeof(int), 0);
            if (res == -1) {
                perror("Erreur réception entier");
            }
            else if (res == 0) {
                printf("Réception impossible, connection close\n");
                        exit(1);
                        close(clientSocket);
            }
            else {
                printf("[SERVEUR/ITERATIF] Taille du prochain message %i octets\n", size);
                char* message2 = (char*) malloc(size);
                res = recv(clientSocket, message2, size, 0);

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
        close(clientSocket);*/

        pthread_t threads[1]; //tableau qui sert à stocker les identifiants de threads (pthread_t sont les identifiants) 
        
        infos.id = i;
        struct paramsFonctionThread *infos;
        infos->id = clientSocket;
        infos->sockclientaddr = clientSockAddr.sin_addr;
        infos->sockclientport = clientSockAddr.sin_port;
        // création des threards 
        // Le passage de paramètre est fortement conseill� (�viter les variables globles).
        if (pthread_create(&threads, NULL, fonctionThread, &infos) != 0){ //tous les threads appellent fonctionThread, thread[i] récupère l'identifiant système du thread qui vient d'être créée        perror("erreur creation thread");
            exit(1);}
        char c; 
        c = fgetc(stdin); //sert à poursuivre l'exécution
  }

   /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
   
   // On pourrait aussi faire close() mais shutdown() est plus sécurisée et plus pratique à manipuler.
   close(ds);
   shutdown(ds, SHUT_RDWR);

   printf("[SERVEUR] Sortie.\n");
   return 0;
}