#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>


// ce programme est à compléter en fonction des instructions à
// recevoir depuis un processus serveur TCP distant. L'IP et le numéro de
// port du serveur sont fournis en début d'épreuve par les enseignants
// et sont à passer en paramètres de votre programme.

// Une instruction est sous forme de chaine de caractères (caractère
// de fin de chaine inclus) dont la taille (de type int) est à
// recevoir juste avant le texte de l'instruction. Donc, une
// instruction implique deux messages : un premier message qui est la
// taille (en nombre d'octets) du texte de l'instruction et un second
// message qui est le texte de l'instruction.

// Après execution d'une instruction, votre programme recoit un
// feedback avec l'instruction suivante. Là aussi, deux messages sont
// à recevoir : le premier qui est la taille (de type int) en nombre
// d'octets du second mesage qui est une chaine de caractères
// (caractère de fin inclus).

// Remarque 1 : un message de type chaine de caractères ne depassera jamais 6000 octets.
#define MAX_BUFFER_SIZE 6000
//TODO : Au final je ne l'utilise, il me faudrait l'utiliser quand je renvois un message

/* Fonction custom de réception TCP vu en TP */
int sendTCP(int sock, char* msg, int sizeMsg) {
    int snd; //On peut aussi utiliser ssize_t si on est tombé dans la soupe de C étant petit
    int totalSent = 0;
    while(sizeMsg) { //Tant que l'on reçoit des données, on pourrait juste mettre while(sizeMsg) si on est tombé dans la soupe de C étant petit
    snd = send(sock, msg+totalSent, sizeMsg, 0);

    //printf("sendTCP :: On a envoyé %i octets\n", snd);

    //Test
    //Erreur d'envoi
    if(snd<0) return -1;

    //Socket fermée
    if(snd==0) return 0;

    totalSent+=snd;
    sizeMsg-=snd;
    }
    //Succés
    return 1;
}

/* Fonction custom de réception TCP vu en TP */
int recvTCP(int sock, char* msg, int sizeMsg) {
    int rcv; //On peut aussi utiliser ssize_t si on est tombé dans la soupe de C étant petit
    int totalRcv = 0;
    while(sizeMsg) { //Tant que l'on reçoit des données, on pourrait juste mettre while(sizeMsg) si on est tombé dans la soupe de C étant petit
    rcv = recv(sock, msg+totalRcv, sizeMsg, 0);

    //printf("recvTCP :: On a reçu %i octets\n", rcv);

    //Test
    //Erreur de réception
    if(rcv<0) return -1;

    //Socket fermée
    if(rcv==0) return 0;

    totalRcv+=rcv;
    sizeMsg-=rcv;
    }
    //Succés
    return 1;
}

int main(int argc, char *argv[]) {

    if (argc != 4){
        printf("Utilisation : %s ip_serveur port_serveur param3 \n param3 est un entier dont la signification sera fournie par une instruction. En attendant cette instruction, passer n'importe quelle valeur\n", argv[0]);
        exit(1);
    }
    // première étape : se connecter au serveur et recevoir la première
    // instruction (lire commentaire plus haut). Bien évidement, il est
    // necessaire d'afficher le message reçu pour prendre connaissance
    // des instructions à suivre pour compléter votre programme.

    /* Étape 1 : Création socket IPV4 TCP */
    int ds = socket(PF_INET,SOCK_STREAM, 0);

    /* Verif */
    if (ds == -1){
        printf("Client :: Échec création socket\n");
        exit(1);
    } else {
        printf("Client :: Socket crée\n");
    }

    /* Étape 2 : Designer socket serveur */
    struct sockaddr_in adrServ;
    adrServ.sin_addr.s_addr = inet_addr(argv[1]) ;// inet_addr("162.38.80.28")
    adrServ.sin_family = AF_INET;
    adrServ.sin_port = htons( (short) atoi (argv[2]));  //htons(46223)

    int lgAdr = sizeof(struct sockaddr_in);

    /* Étape 3 : Demande de connexion au serveur */
    int conn = connect(ds, (struct sockaddr *)&adrServ, lgAdr );
    if (conn < 0){
        perror("Client :: Erreur de connexion\n");
        close(ds);
        exit(1);
    }

    printf("Client : Demande de connexion reussie \n");
    printf("Demande de réception de la première instruction\n");

    //Premier message
    //D'abord on reçoit la taille du string avec le \0 sous forme de int
    int sizeMsg = 0;
    int rcv = recv(ds, (char*)&sizeMsg, sizeof(int), 0);
    //On vérifie la bonne arrivée du message
    //Erreur réception
    if(rcv<0){
        perror("Client :: Impossible de recevoir la taille du message\n");
        close(ds);
        exit(1);
    }
    //Socket fermée
    if(rcv==0){
        printf("Client :: Serveur déconnecté\n");
        close(ds);
        exit(1);
    }
    //Ensuite on va créer un tableau vide de la taille du message à recevoir
    char message[sizeMsg];
    //Puis on reçoit l'instruction
    rcv = recvTCP(ds, message, sizeMsg);
    //On vérifie la bonne arrivée du message
    //Erreur réception
    if(rcv<0){
        perror("Client :: Impossible de recevoir l'instruction\n");
        close(ds);
        exit(1);
    }
    //Socket fermée
    if(rcv==0){
        printf("Client :: Serveur déconnecté\n");
        close(ds);
        exit(1);
    }
    printf("%s\n", message);

    /* Vous venez de recevoir un message de type chaine de caracteres. L'etape
    suivante est de renvoyer ce meme message (et uniquement ce message) a
    l'expediteur, ensuite de recevoir une nouvelle instruction. */
    //On renvois la chaine de caractère comme demandé
    printf("envoi message 2\n");
    int snd = sendTCP(ds, message, sizeMsg);
    if(snd<0){
        perror("Client :: Erreur de send \n");
        close(ds);
        exit(1);
    }
    if(snd==0){
        printf("Client ::  Serveur déconnecté\n");
        close(ds);
        exit(1);
    }
    printf("Demande de réception de la seconde instruction\n");
    //D'abord on reçoit la taille du string avec le \0 sous forme de int
    int sizeMsg2 = 0;
    rcv = recv(ds, (char*)&sizeMsg2, sizeof(int), 0);
    //On vérifie la bonne arrivée du message
    //Erreur réception
    if(rcv<0){
        perror("Client :: Impossible de recevoir la taille du message\n");
        close(ds);
        exit(1);
    }
    //Socket fermée
    if(rcv==0){
        printf("Client :: Serveur déconnecté\n");
        close(ds);
        exit(1);
    }
    //Ensuite on va créer un tableau vide de la taille du message à recevoir
    char message2[sizeMsg2];
    //Puis on reçoit l'instruction
    rcv = recvTCP(ds, message2, sizeMsg2);
    //On vérifie la bonne arrivée du message
    //Erreur réception
    if(rcv<0){
        perror("Client :: Impossible de recevoir l'instruction\n");
        close(ds);
        exit(1);
    }
    //Socket fermée
    if(rcv==0){
        printf("Client :: Serveur déconnecté\n");
        close(ds);
        exit(1);
    }
    printf("%s\n", message2);

    /* Bravo, vous avez desormais acces a la prochaine etape.
    Prochaine etape : le client devient aussi serveur TCP.
    A la suite de ce qui est deja fait,  mettre en place une
    socket d'ecoute TCP avec un numero de port a passer en parametre
    du programme (param3). Apres la mise en ecoute de cette socket,
    envoyer au serveur distant le numero de port choisi (type short)
    puis accepter une demande de connexion. Apres l'acceptation d'une
    demande de connexion d'un nouveau client, votre programme doit recevoir
    une nouvelle instruction envoyée par ce client. Ce n'est donc plus le
    serveur distant qui envoit des instructions mais un client (faire attenton
    à utiliser la bonne socket. Vous devez fermer toute socket une fois qu'elle
    ne sera plus utile */

    //On va créer un autre socket en TCP IPV4
    int ds2 = socket(PF_INET,SOCK_STREAM, 0);
    /* Verif */
    if (ds2 == -1){
    printf("Serveur :: Échec création socket\n");
        close(ds);
        close(ds2);
        exit(1);
    } else {
        printf("Serveur :: Socket crée\n");
    }
    //On nomme la socket du serveur
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY ; // INADDR_ANY
    server.sin_port = htons( (short) atoi (argv[3]));

    if(bind(ds2, (struct sockaddr *) &server, sizeof(server)) < 0){
        perror("Serveur :: Échec du bind\n");
        close(ds);
        close(ds2);
        exit(1);
    }
    printf("Serveur :: Nommage ok\n");

    //Mise en écoute
    int ecoute = listen(ds2, 5);
    if (ecoute < 0){
        printf("Serveur :: Échec de la mise en écoute\n");
        close(ds);
        close(ds2);
        exit(1);
    }
    printf("Serveur :: Mise en écoute terminée\n");

    //On va envoyer notre port au serveur
    short portSend = (short) atoi (argv[3]);
    snd = send(ds, (char*)&portSend, sizeof(short), 0);
    if(snd<0){
        perror("Client :: Erreur de send \n");
        close(ds);
        close(ds2);
        exit(1);
    }
    if(snd==0){
        printf("Client ::  Serveur déconnecté\n");
        close(ds);
        close(ds2);
        exit(1);
    }
    printf("Client :: Port %s envoyé au serveur\n", argv[3]);
    printf("Serveur :: Attente de réception\n");

    //Ici on va gérer plusieurs clients
    int i = 4;

    while(i) {
        printf("Client %i \n", i);
        struct sockaddr_in adCv;
        socklen_t lgCv = sizeof(struct sockaddr_in);

        int dsCv = accept(ds2, (struct sockaddr *) &adCv, &lgCv);
        if (dsCv < 0){
            perror("Serveur :: accept a échoué\n");
            close(ds);
            close(ds2);
            close(dsCv); //TODO: A vérifier
            exit(1);
        }
        printf("Serveur: le client %s:%d est connecté \n", inet_ntoa(adCv.sin_addr), ntohs(adCv.sin_port));

        //Maintenant que nous sommes connecté, nous attendons de recevoir une instruction d'un client
        int sizeMsg3 = 0;
        //Comme d'habitude, d'abord la taille en int
        rcv = recv(dsCv, (char*)&sizeMsg3, sizeof(int), 0);
        if(rcv<0){
            perror("Serveur :: Impossible de recevoir la taille de l'instruction\n");
            i-=1;
            close(dsCv);
            continue;
        }
        //Socket fermée
        if(rcv==0){
            printf("Serveur :: Client déconnecté\n");
            i-=1;
            close(dsCv);
            continue;
        }
        char message3[sizeMsg3];
        rcv = recvTCP(dsCv, message3, sizeMsg3);
        if(rcv<0){
            perror("Serveur :: Impossible de recevoir l'instruction\n");
            i-=1;
            close(dsCv);
            continue;
        }
        //Socket fermée
        if(rcv==0){
            printf("Serveur :: Client déconnecté\n");
            i-=1;
            close(dsCv);
            continue;
        }
        printf("message 3 : %s\n", message3);
        //Normalement nous n'aurons plus besoin de la socket client, on la ferme
        //close(ds);

        /* A present, votre serveur doit recevoir un message de type chaine de
        caractères (caractere de fin inclus) a afficher et a renvoyer a l'expediteur.
        Pour recevoir ce message correctement, il faut recevoir sa taille (type int)
        juste avant. L'étape d'apres sera de recevoir un nouveau feedback avec une
        nouvelle instruction. */
        int sizeMsg4 = 0;
        //Comme d'habitude, d'abord la taille en int
        rcv = recv(dsCv, (char*)&sizeMsg4, sizeof(int), 0);
        if(rcv<0){
            perror("Serveur :: Impossible de recevoir la taille de l'instruction\n");
            i-=1;
            close(dsCv);
            continue;
        }
        //Socket fermée
        if(rcv==0){
            printf("Serveur :: Client déconnecté\n");
            i-=1;
            close(dsCv);
            continue;
        }
        printf("Taille du message : %i\n", sizeMsg4);
        char message4[sizeMsg4];
        rcv = recvTCP(dsCv, message4, sizeMsg4);
        printf("Serveur :: J'ai reçu %i octets\n", rcv);
        if(rcv<0){
            perror("Serveur :: Impossible de recevoir l'instruction\n");
            i-=1;
            close(dsCv);
            continue;
        }
        //Socket fermée
        if(rcv==0){
            printf("Serveur :: Client déconnecté\n");
            i-=1;
            close(dsCv);
            continue;
        }
        printf("message 4 : %s\n", message4);
        snd = send(dsCv, message4, sizeMsg4, 0);
        if(snd<0){
            perror("Serveur :: Erreur de send \n");
            i-=1;
            close(dsCv);
            continue;
        }
        if(snd==0){
            printf("Serveur ::  Serveur déconnecté\n");
            i-=1;
            close(dsCv);
            continue;
        }
        //On reçois le prochain feedback
        //Au début j'avais fait des fonctions, je regrette de les avoir supprimé là

        close(dsCv);
        i-=1;
        /* Bravo, vous avez atteint la derniere etape. Il reste a modifier votre code
        pour que la partie serveur de votre programme soit capable de traiter 4
        clients, un apres l'autre (serveur iteratif). Quatre clients enverront une
        demande de connexion. Attention, l'un des clients est malicieux et peut se
        deconnecter a n'importe quel moment. Ce comportement ne doit pas arreter votre
        serveur, qui doit pouvoir echanger avec un autre client qui se connecte apres. */

    }

    close(ds);
    close(ds2);
}
