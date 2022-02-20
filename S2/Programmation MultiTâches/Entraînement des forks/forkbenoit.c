#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>//perror
#include <unistd.h>
#include <string.h>

/**
 * Crée des processus tous enfants du même parent.
 *
 * On fait simplement une boucle qui tourne n fois.
 * On sort de la boucle si le processus courant n'est pas le parent,
 * car on ne veut pas que les enfants fork.
 */
void largeur(int nbProc) {
    int parentId = getpid();
    int i = 0;

    printf("Processus parent : %i\n\n", parentId);

    while (i < nbProc && getpid() == parentId) {
        printf("Fork n°%i\n", i);
        int res = fork();

        if (res == -1) {
            perror("Problème fork");
            exit(1);
        }

        i++;
    }

    if (getpid() != parentId) {
        printf("Travail du processus n°%i pendant %i secondes\n", getpid(), i);
        sleep(i);
    }

    while(wait(0) != -1);
    printf("Fin du travail du processus n°%i\n", getpid());
}

/**
 * Crée des processus en ligne, chaque enfant crée un nouveau processus.
 *
 * On fait simplement une boucle qui tourne n fois.
 * On sort de la boucle si le processus courant est le parent, car on
 * veut uniquement que l'enfant fork().
 * 
 * L'id du parent change constamment pour retenir le nouveau parent.
 */
void hauteur(int nbProc) {
    int parentId = getpid();
    int i = 0;

    printf("Processus parent : %i\n\n", parentId);

    do {
        parentId = getpid();
        printf("Fork n°%i\n", i);
        int res = fork();

        if (res == -1) {
            perror("Problème fork");
            exit(1);
        }

        i++;
    } while (i < nbProc && getpid() != parentId);

    printf("Travail du processus n°%i pendant %i secondes\n", getpid(), i);
    sleep(i);

    while(wait(0) != -1);
    printf("Fin du travail du processus n°%i\n", getpid());
}

/**
 * Crée des processus en arbre binaire complet.
 *
 * Cette fois si on le fait en récursif.
 * On applique également une nouvelle méthode pour savoir si un processsus
 * est parent ou enfant. Si le résultat du fork() est 0, c'est que nous
 * sommes dans le processus enfant.
 */
void arbre(int nbProc) {
    printf("Processus actuel : %i\n", getpid());

    if (nbProc != 0) {
        for (int i = 0; i < 2; ++i) {
            int res = fork();

            if (res == -1) {
                perror("Problème fork");
                exit(1);
            }
            else if (res == 0) {
                arbre(nbProc - 1);
                exit(0);
            }
        }
    }

    printf("Travail du processus n°%i pendant 7 secondes\n", getpid());
    sleep(7);

    while(wait(0) != -1);
    printf("Fin du travail du processus n°%i\n", getpid());
}

/**
 * Crée des processus en arbre binaire complet.
 *
 * Chaque processus attend un message du parent et l'affiche une fois reçu 
 */
void arbrePipe(int nbProc, int pipefd[2]) {
    close(pipefd[1]);
    int childsfd[2];
    sleep(1);

    printf("Processus actuel : %i\n", getpid());

    if (nbProc != 0) {
        if (pipe(childsfd) == -1) {
            perror("Problème pipe");
            exit(1);
        }

        int res = fork();
        if (res == -1) {
            perror("Problème fork");
            exit(1);
        }
        else if (res == 0) {
            arbrePipe(nbProc - 1, childsfd);

            while(wait(0) != -1);
            exit(0);
        }
        else {
            close(childsfd[0]);
        }
    }

    printf("Le processus n°%i est en attente d'un message\n", getpid());
    
    char message[250];

    read(pipefd[0], message, 250);
    close(pipefd[0]);

    printf("Le processus n°%i a lu le message : %s\n", getpid(), message);

    if (nbProc != 0) {
        printf("Processus n°%i : message renvoyé aux enfants\n", getpid());

        write(childsfd[1], message, strlen(message) + 1);

        close(childsfd[1]);
    }
}

void tube(int nbProc) {
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("Problème pipe");
        exit(1);
    }

    int res = fork();
    if (res == -1) {
        perror("Problème fork");
        exit(1);
    }
    else if (res == 0) {
        arbrePipe(nbProc, pipefd);
        while(wait(0) != -1);
        exit(0);
    }

    sleep(nbProc + 1);
    printf("Ecrivez un message :");

    char message[250];
    fgets(message, 250, stdin);
    printf("\n");

    close(pipefd[0]);
    write(pipefd[1], message, strlen(message) + 1);
    close(pipefd[1]);
    
    while(wait(0) != -1);
    printf("\nLe processus parent a terminé\n");
}

int  main (int argc, char *argv[]) {
    if (argc != 2) {
        printf("Utilisation : %s nombre_de_processus_fils\n", argv[0]);
        exit(1);
    }

    // largeur(atoi(argv[1]));
    // hauteur(atoi(argv[1]));
    // arbre(atoi(argv[1]));
    tube(atoi(argv[1]));
    
    return 0;
}

