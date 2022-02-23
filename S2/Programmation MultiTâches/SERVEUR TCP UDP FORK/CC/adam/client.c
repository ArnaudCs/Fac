#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>

/* Programme client TCP */

int main(int argc, char *argv[]) {

    /* je passe en paramètre l'adresse de la socket du serveur (IP et
        numéro de port) et un numéro de port à donner à la socket créée plus loin.*/

    /* Je teste le passage de parametres. Le nombre et la nature des
        paramètres sont à adapter en fonction des besoins. Sans ces
        paramètres, l'exécution doit être arrétée, autrement, elle
        aboutira à des erreurs.*/
    if (argc != 5){
        printf("utilisation : %s ip_serveur port_serveur\n", argv[0]);
        exit(1);
    }

    /* Etape 1 : créer une socket */   
    int srv = socket(PF_INET, SOCK_STREAM, 0);
    if (srv == -1){
        perror("[Client] : pb creation socket serveur :\n");
        exit(1);
    }
    printf("[SERVEUR] : creation de la socket réussie \n");
    /* etape 1.2 : nommage de la socket serveur */
    struct sockaddr_in socketsrv;
    socksrv.sin_family = AF_INET;
    socksrv.sin_addr.s_addr = INADDR_ANY;
    socksrv.sin_port = htons((short)atoi(argv[2]));
    res = bind(srv, (struct sockaddr *)&socksrv, sizeof(socksrv));

    if (res == -1){
        perror("[SERVEUR] : pb nommage socket :\n");
        exit(1);
    }
    /* etape 2 : designer la socket du client */
    struct sockaddr_in sockclient;

    int srvListen = listen(srv, 10);
    if (srvListen == -1) {
        perror("Serveur : problème lors de la mise en écoute de la socket");
        exit(1);
    }
    printf("Serveur : socket serveur sur écoute.\n");



    int newConnetion = accept(srv, (struct sockaddr *)&sockclient, &size);
    if(newConnetion == -1) {
        perror("[SERVEUR] Erreur lors d'une connexion entrante");
        exit(1);
    }
    printf("Serveur : connexion du client (%s:%i)\n", inet_ntoa(sock_clt2.sin_addr), ntohs((short) sock_clt2.sin_port));


printf("###################  PARTIE SERVEUR ITERATIF  ######################\n");
while (1) {
    struct sockaddr_in sock_cltTCP;
    int newConnectionTCP = accept(srv, (struct sockaddr*)&sock_cltTCP, &size);

    if (newConnectionTCP == -1 || newConnectionTCP == 0) {
        perror("[Serveur] : problème lors de la connection d'un client");
    }

    int pid = fork();

    if (pid == 0) {
        
        printf("[SERVEUR] Le client connecté est %s:%i.\n", inet_ntoa(sock_cltTCP.sin_addr), ntohs(sock_cltTCP.sin_port));
        
        int tailleTCP =0;
        int resTailleTCP = recvTCP(newConnectionTCP,&tailleTCP,sizeof(int));
        if (resTailleTCP == -1 || resTailleTCP == 0){
            perror("[Serveur] : pb réception taille msg ou client fermé:\n");
            
        }
        else
        {
            char msgTCP[tailleTCP];
            int resMsgTCP = recvTCP(newConnectionTCP,msgTCP,tailleTCP);
            if (resMsgTCP == -1 || resMsgTCP == 0){
                perror("[Serveur] : pb réception msg ou client fermé :\n");
                
            }
            else
            {
                printf("[Serveur] : nombre d'octet : %i, message reçu : %s\n", resTailleTCP, msgTCP);
            }
        }

        int tailleTCP2 =0;
        int resTailleTCP2 = recvTCP(newConnectionTCP,&tailleTCP2,sizeof(int));
        if (resTailleTCP2 == -1 || resTailleTCP2 == 0){
            perror("[Serveur] : pb réception taille msg ou client fermé:\n");
            
        }
        else
        {
            char msgTCP2[tailleTCP2];
            int resMsgTCP2 = recvTCP(newConnectionTCP,msgTCP2,tailleTCP2);
            if (resMsgTCP2 == -1 || resMsgTCP2 == 0){
                perror("[Serveur] : pb réception msg ou client fermé :\n");
                
            }
            else
            {
                printf("[Serveur] : nombre d'octet : %i, message reçu : %s\n", resTailleTCP2, msgTCP2);
            }
        }
        
        close(newConnectionTCP);
    }
    else
    {
        close(newConnectionTCP);
    }
    
  }
    close(srv);
    printf("[CLIENT] Sortie.\n");
    return 0;
}
