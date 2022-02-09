#include <stdio.h>//perror
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>//close
#include <stdlib.h>
#include <string.h>


#define MAX_BUFFER_SIZE 146980


int main(int argc, char *argv[])
{
  /* etape 0 : gestion des paramètres si vous souhaitez en passer */
  
  /* etape 1 : creer une socket d'écoute des demandes de connexions*/
	   int ds = socket(PF_INET, SOCK_STREAM, 0);
	   if (ds == -1) {
	      perror("[SERVEUR] Erreur lors de la création de la socket ");
	      exit(1);
	   }
	   printf("[SERVEUR] Création de la socket réussie.\n");

  
  /* etape 2 : nommage de la socket */
	  struct sockaddr_in ad;
      socklen_t len = sizeof(ad);
      ad.sin_family = AF_INET;      
      ad.sin_addr.s_addr = INADDR_ANY;

  /* etape 3 : mise en ecoute des demandes de connexions */
    ad.sin_port = htons(atoi(argv[1]));
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
	  
	res = listen(ds, 10);
	if (res == -1){
	  printf("Erreur lors du passage en mode écoute");
      exit(1);
	}
	else {printf("Passage en mode écoute réussie\n");}
	 
  /* etape 4 : plus qu'a attendre la demadne d'un client */
  struct sockaddr_in sockClient;
  socklen_t lgAdr = sizeof(struct sockaddr_in);
  int dsclient = accept(ds, (struct sockaddr*)&sockClient, &lgAdr);

  int totalRecv = 0; // un compteur du nombre total d'octets recus d'un client
  
  /* le protocol d'echange avec un client pour recevoir un fichier est à définir. Ici seul un exemple de code pour l'écriture dans un fichier est founi*/

  char* filepath = "./reception/test"; // cette ligne n'est bien-sur qu'un exemple et doit être modifiée : le nom du fichier doit être reçu.

  // On ouvre le fichier dans lequel on va écrire
  FILE* file = fopen(filepath, "wb");
  if(file == NULL){
    perror("Serveur : erreur ouverture fichier: \n");
    exit(1);  
  }
  int totalSent = 0; // variable pour compter le nombre total d'octet effectivement envoyés au serveur du début à la fin des échanges.
  char * buffer ="";
  while()
  size_t written = fwrite(buffer, sizeof(char), strlen(buffer)+1, file);
  if(written < strlen(buffer)+1){  // cette ligne est valide uniquement pour ce simple exemple
    perror("Serveur : Erreur a l'ecriture du fichier \n");
    fclose(file); 
  }
  printf("Serveur : ecriture dans fichier reussie. Vous pouvez vérifier la création du fichier et son contenu.\n");
 
    printf("le poids du fichier txt1 est de %d octets.\n\n\n", poidsTxt1);
  // fermeture du fichier
  fclose(file);
    
  printf("Serveur : c'est fini\n");
}








