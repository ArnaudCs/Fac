#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
/* Programme serveur du tp noté */

void afficheErreur() {
    printf("errno: %d , %s\n", errno, strerror(errno));
}

void quitterFork(int sock) {
    close(sock);
    exit(1);
}

int creerSocket() {
    int ds = socket(PF_INET, SOCK_STREAM, 0);
    if (ds == -1){
        perror("Serveur : pb creation socket :");
        exit(1); 
    }
    printf("Serveur : creation de la socket réussie \n");
    return ds;
}

struct sockaddr_in nommerSocket(char* port, int ds) {
    struct sockaddr_in ad;
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY;
    if(atoi(port) != -1) {
        ad.sin_port = htons((short) atoi(port));
    }
    int res;
    res = bind(ds, (struct sockaddr*)&ad, sizeof(ad));
    if (res == 0) {
        printf("Socket nommée avec succès\n");
    } else {
        printf("Socket non nommée : %i \n", res);
        afficheErreur();
        exit(1);
    }
    return ad;
}

int sendTCP(int sock, void* msg, int sizeMsg) {
    int res;
    int sent=0;
    //boucle d'envoi
    while(sent < sizeMsg) {
        //printf("Reste à envoyer: %i (res %i) envoyé %i\n",sizeMsg-sent, res, sent);
        res = send(sock, msg+sent, sizeMsg-sent, 0);
        sent += res;
        if (res == -1) {
            printf("Problème lors de l'envoi du message\n");
            afficheErreur();
            quitterFork(sock);
            return -1;
        }
        if (res == 0) {
            printf("Déconnexion\n");
            quitterFork(sock);
            return 0;
        }
    }
    return sent;
}

int recvTCP(int sock, void* msg, int sizeMsg) {
    int res;
    int received=0;
    //boucle de réception
    while(received < sizeMsg) {
        res = recv(sock, msg+received, sizeMsg-received, 0);
        received += res;
        if (res == -1) {
            printf("Problème lors de la réception du message\n");
            afficheErreur();
            quitterFork(sock);
            return -1;
        } else if (res == 0) {
            printf("Déconnexion\n");
            quitterFork(sock);
            return 0;
        }
    }
    return received;
}

int recv2TCP(int sock, void* msg, int sizeMsg) {
    int taille;
    recvTCP(sock, &taille, sizeof(int));
    if (taille > sizeMsg) {
        printf("problème, buffer trop petit!\n");
        quitterFork(sock);
        return -1;
    }
    return recvTCP(sock, msg, taille);
}

int send2TCP(int sock, void* msg, int sizeMsg) {
    sendTCP(sock, &sizeMsg, sizeof(int));
    return sendTCP(sock, msg, sizeMsg);
}

int main(int argc, char *argv[]) {

    if (argc != 2){
        printf("utilisation : %s port_serveur\n", argv[0]);
        exit(1);
    }
    
    int ds = creerSocket();
    struct sockaddr_in ad = nommerSocket(argv[1],ds);
    // afficher port
    socklen_t sizeAd = sizeof(ad);
    getsockname(ds, (struct sockaddr*) &ad, &sizeAd);
    printf("port: %i\n",ntohs(ad.sin_port));

    // écouter
    int nbMaxAttente = 10;
    int resListen = listen(ds, nbMaxAttente);
    if (resListen == -1) {
        printf("Problème lors de l'écoute\n");
        printf("errno: %d , %s\n", errno, strerror(errno));
        exit(1);    
    }

    // préparer quelques ingrédients
    struct sockaddr_in sockClient;
    socklen_t lgAdr;
    char strIp[INET_ADDRSTRLEN];
    char buffer[6000];
    int size_max = 6000;
    int dsClient;

    // boucle principale
    while (1) {
        //accepter une connexion
        dsClient = accept(ds, (struct sockaddr*)&sockClient, &lgAdr);

        if (dsClient == -1) {
            printf("Problème lors de la connexion\n");
            afficheErreur();
            continue;
        } else {
            inet_ntop(AF_INET, &sockClient.sin_addr, strIp, INET_ADDRSTRLEN);
            printf("Connexion à l'adresse %s\n",strIp);

            if(fork() == 0) {
                //on est l'enfant
                close(ds);
                // -> envoi de l'étape 2
                printf("Envoi étape 2\n");
                char* etape2 = "Vous venez de recevoir un message de type chaine de caracteres. L'etape suivante est de renvoyer ce meme message (et uniquement ce message) à l'expediteur, ensuite de recevoir une nouvelle instruction.";
                send2TCP(dsClient, etape2, strlen(etape2)+1);
                // -> réception de l'étape 2
                printf("Réception étape 2\n");
                int resEtape2 = recvTCP(dsClient, buffer, strlen(etape2)+1);
                if (strlen(etape2)+1 != resEtape2) {
                    printf("Reception de l'étape 2 ratée");
                    quitterFork(dsClient);
                }
                // -> envoi de l'étape 3
                printf("Envoi étape 3\n");
                char* etape3 = "Bravo, vous avez desormais acces a la prochaine etape.\nProchaine etape : le client devient aussi serveur TCP. A la suite de ce qui est deja fait,  mettre en place une socket d'ecoute TCP avec un numero de port a passer en parametre du programme (param3). Apres la mise en ecoute de cette socket, envoyer au serveur distant le numero de port choisi (type short) puis accepter une demande de connexion. Apres l'acceptation d'une demande de connexion d'un nouveau client, votre programme doit recevoir une nouvelle instruction envoyée par ce client. Ce n'est donc plus le serveur distant qui envoit des instructions mais un client (faire attention à utiliser la bonne socket). Vous devez fermer toute socket une fois qu'elle ne sera plus utile.";
                send2TCP(dsClient, etape3, strlen(etape3)+1);
                // -> réception numéro port
                short port;
                printf("Réception port\n");
                int resPort = recv(dsClient, &port, sizeof(short), 0);
                if (resPort == -1) {
                    printf("Problème à la réception du port\n");
                    afficheErreur();
                    quitterFork(dsClient);
                } else if (resPort == 0) {
                    printf("Deconnexion\n");
                    quitterFork(dsClient);
                }
				printf("Port reçu : %i\n", port);
                // on va connecter quatre fois
                // anciennement 2 fork
                int clientNumber = 4;
                while (clientNumber) {
                    printf("Création client n° %i\n",clientNumber);
                    // -> connexion au numéro indiqué
                    int dsE3 = socket(PF_INET, SOCK_STREAM, 0);
                    if (dsE3 == -1){
                        perror("Serveur : pb creation socket :");
                        quitterFork(dsClient);
                        exit(1); 
                    }
                    printf("Connexion: creation de la socket réussie \n");
                    close(dsClient);
                    struct sockaddr_in sockE3;
                    sockE3.sin_family = AF_INET;
                    int resConvertAddr = inet_pton(AF_INET, strIp, &(sockE3.sin_addr));
                    if(resConvertAddr == 1) {
                        printf("Adresse IP correctement convertie\n");
                    } else {
                        printf("Problème à la conversion de l'adresse IP\n");
                        afficheErreur();
                        quitterFork(dsE3);
                    }
                    sockE3.sin_port = htons(port);
                    inet_ntop(AF_INET, &sockE3.sin_addr, strIp, INET_ADDRSTRLEN);
                    printf("Connexion à l'adresse %s\n",strIp);
                    // tentative de connexion
                    int resConnexion = connect(dsE3, (struct sockaddr *)&sockE3, lgAdr);
                    if (resConnexion == 0) {
                        printf("Connexion réussie\n");
                    } else {
                        printf("Connexion impossible\n");
                        afficheErreur();
                        quitterFork(dsE3);
                    }
                    // -> envoi de l'étape 4
                    printf("Envoi étape 4\n");
                    char* etape4 = "A present, votre serveur doit recevoir un message de type chaine de caractères (caractere de fin inclus) a afficher et a renvoyer a l'expediteur.\nPour recevoir ce message correctement, il faut recevoir sa taille (type int) juste avant. L'étape d'apres sera de recevoir un nouveau feedback avec une nouvelle instruction.";
                    // un des fork s'arrête brutalement
                    if (clientNumber == 2) {
                        close(dsE3);
                        clientNumber--;
                        continue;
                    }
                    send2TCP(dsE3, etape4, strlen(etape4)+1);
                    // -> envoi de l'étape 5
                    char* etape5 = "Bravo, vous avez atteint la derniere etape. Il reste a modifier votre code pour que la partie serveur de votre programme soit capable de traiter 4 clients, un apres l'autre (serveur iteratif). Quatre clients enverront une demande de connexion. Attention, l'un des clients est malicieux et peut se deconnecter a n'importe quel moment. Ce comportement ne doit pas arreter votre serveur, qui doit pouvoir echanger avec un autre client qui se connecte apres.";
                    if (clientNumber == 1) {
                        etape5 = "Je crois que vous êtes prêt pour le tp noté. Bien joué!";
                    }
                    printf("Envoi étape 5\n");
                    send2TCP(dsE3, etape5, strlen(etape5)+1);
                    close(dsE3);
                    clientNumber--;
                }
                exit(0);

            } else {
                close(dsClient);
            }

        }

        
    }

    close(ds);
    while(wait(0)!=-1);
    printf("Serveur : je termine\n");
    return 0;

}