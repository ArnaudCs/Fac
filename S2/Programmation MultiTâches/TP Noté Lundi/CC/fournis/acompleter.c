#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>


/* Ce programme est à compléter et à déposer sur Moodle (c'est le seul fichier à déposer).

   Lire attentivement les instructions en commentaires pour compléter correctement ce programme.
 
   Les principales étapes de ce programme sont :
 
   1) demander à un serveur UDP une liste d'adresses de serveurs TCP existants.
 
   2) se connecter à l'un de ces serveurs TCP et échanger avec ce serveur.
 
   3) faire évoluer votre programme pour ajouter un serveur TCP qui échangera avec un exécutable client fourni.
 
   4) modifier votre programme pour qu'il soit capable de traiter plusieurs clients de manière itérative.
 
   5) modifier votre programme pour qu'il soit capable de traiter plusieurs clients  simultanéments (en utilisant la fonction fork()).
 
   Attention : vous devez déposer un code qui compile. Exemple : si vous êtes à l'étape 4 qui n'est pas fonctionnelle, la mettre en commentaire pour que toutes les étapes d'avant soient validées.
  
*/

  #define MAXSRV 100
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

   int receiveTCP(int connectedSocket, char *message, size_t maxMessageSize) {
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

  // cette structure est le type du message à envoyer au serveur UDP
  struct nomClient{
  short type; // doit être égal à 1
  char hostname[50];
  };

  typedef struct nomClient sNomClient;

  int main(int argc, char *argv[]) {

  if (argc != 4){
    printf("Utilisation : %s ip_serveurUDP port_serveurUDP param3 \n param3 est un entier dont la signification sera fournie par une question. En attendant cette question, passer n'importe quelle valeur\n", argv[0]);
    exit(0);
  }
  
  /* Etape 1 : envoyer un message au serveur UDP et recevoir une réponse :
  
     1) le message à envoyer est de type sNomClient. Il permet d'envoyer le nom de la machine sur laquelle s'exécute votre client. Voir plus loin le code fourni pour vous aider.
   
     2) le message à recevoir est une liste d'élements de type sock_addr_in. Chaque élément représente l'adresse d'un serveur TCP existant.
   
  */
  
  int dSUDP = socket(PF_INET, SOCK_DGRAM, 0);
  
  if (dSUDP < 0){
    perror("Erreur a la creation de la socket :");
    return 1;
  }     

  struct sockaddr_in sockClient;
  sockClient.sin_family = AF_INET;
  sockClient.sin_addr.s_addr = INADDR_ANY;
  sockClient.sin_port = htons((short) atoi(argv[3]));

  int nomSock = bind(dSUDP, (struct sockaddr*)&sockClient, sizeof(sockClient));
  if (nomSock == -1){
   perror("Client : pb creation liaison :");
   exit(1);
  }
  printf("Bind réussi\n");
 
  // designer la socket distante du serveur UDP 
  struct sockaddr_in aD; 
  aD.sin_family = AF_INET; // complétez 
  aD.sin_addr.s_addr = inet_addr(argv[1]);
  aD.sin_port = htons(atoi(argv[2]));
  //socklen_t lgA = sizeof(struct sockaddr_in);
  //...
 
  // Le code suivant prépare le message à envoyer au serveur UDP.
  sNomClient nom;
  nom.type = 1;
  nom.hostname[0]='\0';
  char h[50];
  gethostname(h, 50); // récupère le nom de la machine que vous utilisez.
  strcat(nom.hostname, h);
  // Le message à envoyer au serveur UDP est construit.
  // Envoyer ce message au serveur UDP
  int res = sendto(dSUDP, nom.hostname, sizeof(nom.hostname)+1, 0, (struct sockaddr*)&aD, sizeof(aD));
  if(res == -1){
  printf("Erreur lors de l'envoi du message");
  exit(1);
  }

  /*1. IP 8.0.0.0, port 0 
  Choisir un numero de serveur (1 ou 2 ou ... ): */
  // ...

  // la réponse à recevoir est a stocker dans le tableau suivant. votre programme recevra au plus 50 adresses de serveurs TCP
  struct sockaddr_in reponse[50];
  // ...

  /* affichage de la liste des adresses recues et demande de choix d'un
     serveur. On suppose qu'au moins un serveur tourne. En l'absence d'un serveur, on termine proprement */

  printf("Les serveurs TCP existants : \n"); 
  for(int i = 0; i < 50; i++) {// Attention : une modification doit être faite ici.
    printf("%d. IP %s, port %d \n", i+1, inet_ntoa(reponse[i].sin_addr),  ntohs(reponse[i].sin_port));}
    
  printf("Choisir un numero de serveur (1 ou 2 ou ... ): \n");
  int numS;
  scanf("%d", &numS);
  printf("Vous avez choisi le numéro %i : IP %s, port %i \n", numS, inet_ntoa(reponse[numS-1].sin_addr), ntohs(reponse[numS-1].sin_port));

  // A présent, plus besoin d'échanger avec le serveur UDP.
  close(dSUDP);
  shutdown(dSUDP, SHUT_RDWR);
 //...
 
  /* Etape 2 : Echanger avec le serveur TCP choisi*/
  int dSTCP= socket(PF_INET, SOCK_STREAM, 0);
  
  if (dSUDP < 0){
    perror("Erreur a la creation de la socket :");
    return 1;
  }
  printf("[Client] : creation de la socket réussie \n");

  /* etape 1.2 : nommage de la socket client */
  struct sockaddr_in sockclient;
  socklen_t size = sizeof(struct sockaddr_in);
  sockclient.sin_family = AF_INET;
  sockclient.sin_addr.s_addr = INADDR_ANY;
  //sock_clt.sin_port = htons((short)atoi(argv[3]));
  
  //etape 2 : designer la socket du serveur
  struct sockaddr_in sock_srv;
  sock_srv.sin_family = AF_INET;
  sock_srv.sin_addr.s_addr = atoi(inet_ntoa(reponse[numS-1].sin_addr));
  sock_srv.sin_port = ntohs(reponse[numS-1].sin_port);
  
  printf("Demande de connexion à : %s : %i\n",inet_ntoa(reponse[numS-1].sin_addr), ntohs(reponse[numS-1].sin_port)); 
  /* etape 3 : demander une connexion */
  int dsConnect = connect(dSTCP, (struct sockaddr*)&sock_srv, sizeof(sock_srv));
  if (dsConnect == -1){
      perror("[Client] : pb connexion serveur :\n");
      exit(1);
  }
  printf("[Client] : connexion réussie avec le serveur\n");
  
     /*Après connexion :
 
     1) recevoir une chaîne de caractères dont la taille est connue par le serveur (à vous d'en déduire ce qui est nécecessaire de faire) puis afficher le message reçu.*/
     printf("Attente d'un message\n");
     res = recv(dSTCP, &size, sizeof(int), 0);
     if (res == -1) {
	perror("Erreur réception entier");
     }
     else if (res == 0) {
	printf("Réception impossible, connection close\n");
     }
     else {
	printf("Taille du prochain message %i octets\n", size);
	char* message = (char*) malloc(size);
	res = recv(dSTCP, message, size, 0);

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
     /*2) recevoir un entier. 
     int entier;
     res = receiveTCP(dsConnect, entier, 100);
     if(res == -1){
 	printf("Erreur lors de la réception de l'entier\n");
     }
     Remarque : le serveur peut mettre fin à l'échange avec votre client à n'importe quel moment. Il faut donc bien prévoir ce cas.
   
     3) termine les échanges avec le serveur TCP.
      
  */ 
 
 
 // A vous de jouer
 // ...
 
 

  
  // L'étape suivante est indépendante de la précédente. Donc, elle peut s'exécuter même en cas d'erreurs / fermetures de scokets lors des précédents échange.
  
  /* Etape 3 : Mettre en place un serveur TCP. Ensuite passer à l'étape 4 : Modifier ce serveur pour qu'il soit itératif. Enfin, l'étape 5 : modifier le serveur pour qu'il soit concurrent (en utilisant la fonction fork()). 
 
     Pour l'étape 3, le serveur a un numéro de port qui est le dernier paramètre de votre programme. Ce serveur doit faire ce qui suit pour chaque client :
 
     1) recevoir un tableau de 320000 entiers (int) envoyé par ce client
     2) renvoyer le tableau reçu au client.
     3) recevoir en réponse une chaine de caractères dont la taille est connue par le client (à vous de savoir ce qu'il est nécessaire de faire pour l'obtenir) 
     4) terminer l'échange avec ce client
     
     Pour tester votre serveur, executez le le binaire ./bin/clients fourni. Ce dernier lancera 5 clients.
  */ 
  
 // A vous de jouer.
    
}
  
