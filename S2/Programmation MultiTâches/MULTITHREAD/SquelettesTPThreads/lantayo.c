#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "calcul.h"

void *team(void *params); void *player(void *params);

/** 
 * Structure qui gère les variables partagées des équipes:
 *  - 1 condition `cond` (pour savoir si le compteur entre les équipes est similaire)
 *  - Un tableau d'entier `fillRate` correspondant au taux de remplissage du seau de chaque équipe.
 *  - Un tableau d'entiers `playerTurn` pour savoir c'est le tour de quel joueur pour quelle équipe.
 *  - 1 verrou `lock` pour accéder à ce tableau d'entiers.
 **/
 
struct teamSharedVariables {
    int *fillRate;
    int *playerTurn;

    pthread_cond_t  cond;
    pthread_mutex_t lock;
};
typedef struct teamSharedVariables TeamSharedVariables;

/**
 * Structure qui gère les arguments passés aux équipes :
 *  - `shared` les variables partagées des équipes.
 *  - 1 entier `lenPlayers` le nombre de joueur de chaque équipe.
 *  - 1 entier `len` qui représente le nombre total d'équipes.
 *  - 1 entier `threshold` qui représente le seuil de remplissage requis.
 *  - 1 entier `teamNum` qui représente le numéro de l'équipe.
 **/
struct teamParams {
    int lenPlayers;
    int len;
    int threshold;
    int teamNum;
    TeamSharedVariables *shared;
};
typedef struct teamParams TeamParams;

/**
 * Schéma algorithmique :
 *  - Une fonction qui simule le comportement d'une équipe :
 *      - Envoie un joueur remplir le bambou, puis remplir le sceau
 *      - Il passe le bambou a un autre joueur
 *      - Attend que l'autre équipe soit prête
 * Une condition pour les 2 équipes.
 * Une condition par joueur pour chaque équipe.
 * Un compteur c pour savoir sur quel joueur est le bambou.
 * Tant que c n'est pas identique à celui de l'autre équipe, attendre.
 * Signal au joueur c de lancer le calcul.
 * Quand il a fini, augmente le compteur. c = (c + 1) % nbJoueurs.
 **/
int main(int argc, char **argv) {
    // Trois arguments :
    //  - Le nombre d'équipes.
    //  - Le nombre de joueurs.
    //  - Le seuil de remplissage requis.
    if (argc != 4) {
        printf("Utilisation: %s nombre_equipes nombre_joueurs seuil_remplissage\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int err; 
    int lenTeams = atoi(argv[1]); 
    srand(time(NULL) ^ (getpid() << 16));

    // Instanciation des variables partagées des équipes
    TeamSharedVariables sharedVariables = (TeamSharedVariables){
        .fillRate = (int *)calloc(lenTeams, sizeof(int)),
        .playerTurn = (int *)calloc(lenTeams, sizeof(int)),
    };

    if ((err = pthread_mutex_init(&sharedVariables.lock, NULL)) != 0) {
        printf("Erreur lors de l'instanciation du mutex : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    if ((err = pthread_cond_init(&sharedVariables.cond, NULL)) != 0) {
        printf("Erreur lors de l'instanciation de la condition : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }

    // Stockage des threads créés.
    pthread_t threads[lenTeams];

    // Lancement d'un thread par équipe.
    for (int i = 0; i < lenTeams; ++i) {
        TeamParams *params = (TeamParams *)malloc(sizeof(TeamParams));
        params->len = lenTeams;
        params->lenPlayers = atoi(argv[2]);
        params->threshold = atoi(argv[3]);
        params->teamNum = i;
        params->shared = &sharedVariables;

        if (pthread_create(&threads[i], NULL, team, (void *)params) != 0) {
            perror("Erreur lors de la création du thread d'équipe ");
            exit(1);
        }
    }

    // Attente de tous les threads d'équipes.
    for (int i = 0; i < lenTeams; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Affichage du/des gagnant(s).
    for (int i = 0; i < lenTeams; ++i) {
        if (sharedVariables.fillRate[i] >= atoi(argv[3])) {
            printf("L'équipe %d a rempli son seau !\n", i);
        }
    }

    // On quitte le programme proprement.
    pthread_cond_destroy(&sharedVariables.cond);
    pthread_mutex_destroy(&sharedVariables.lock);
    free(sharedVariables.fillRate);
    return 0;
}

/** 
 * Structure qui gère les variables partagées des joueurs :
 *  - 1 condition `cond` (pour bloquer l'équipe / le joueur si ce n'est pas à lui)
 *  - Un entier `currentPlayer` qui enregistre quel joueur doit jouer.
 *  - Un entier `water` qui enregistre l'eau qui a été récupérée par le joueur.
 *  - 1 verrou `lock` pour accéder à ce tableau d'entiers.
 **/
struct playerSharedVariables {
    int currentPlayer;
    int water;
    pthread_cond_t  cond;
    pthread_mutex_t lock;
};
typedef struct playerSharedVariables PlayerSharedVariables;

struct playerParams {
    char color[5];
    int playerNumber;
    int maxFill;
    PlayerSharedVariables *shared;
};
typedef struct playerParams PlayerParams;

/**
 * Gestion d'une équipe :
 *  - Crée un nombre de threads correspondant au nombre de joueurs.
 *  - Garde un compteur pour savoir quel joueur possède le bambou.
 *  - Quand un joueur fini son tour, incrémente le compteur et attend que toutes les équipes arrivent au même joueur.
 **/
void *team(void *params) {
    TeamParams *args = (TeamParams *)params;
    TeamSharedVariables *shared = args->shared;
    int err;

    char color[5]; 
    sprintf(color, "\033[%dm", (args->teamNum % 10) + 31);
    // Instanciation des variables partagées des joueurs.
    PlayerSharedVariables sharedVariables = (PlayerSharedVariables){
        .currentPlayer = 0,
        .water = 0,
    };

    if ((err = pthread_mutex_init(&sharedVariables.lock, NULL)) != 0) {
        printf("Erreur lors de l'instanciation du mutex : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    if ((err = pthread_cond_init(&sharedVariables.cond, NULL)) != 0) {
        printf("Erreur lors de l'instanciation de la condition : %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }

    // Stockage des threads créés
    pthread_t playerThreads[args->lenPlayers];
    PlayerParams playerParams[args->lenPlayers];

    for (int i = 0; i < args->lenPlayers; ++i) {
        playerParams[i] = (PlayerParams) {
            .playerNumber = i,
            .maxFill = (int)round((double)args->threshold / 2),
            .shared = &sharedVariables,
        };
        strcpy(playerParams[i].color, color);

        if (pthread_create(&playerThreads[i], NULL, player, (void *)&playerParams[i]) != 0) {
            perror("Erreur lors de la création du thread d'équipe ");
            exit(1);
        }
    }
    
    // Boucle de jeu.
    pthread_mutex_lock(&shared->lock);
    while (shared->fillRate[args->teamNum] < args->threshold) {
        pthread_mutex_unlock(&shared->lock);

        // Lancer le joueur à qui c'est le tour.
        pthread_mutex_lock(&sharedVariables.lock);
        sharedVariables.currentPlayer = shared->playerTurn[args->teamNum];
        // Signal à la condition pour réveiller le joueur.
        pthread_cond_broadcast(&sharedVariables.cond);
        pthread_mutex_unlock(&sharedVariables.lock);

        // Attente du retour du joueur.
        pthread_mutex_lock(&sharedVariables.lock);
        pthread_cond_wait(&sharedVariables.cond, &sharedVariables.lock);
        // Récupération du taux d'eau et libération du verrou.
        int water = sharedVariables.water;
        pthread_mutex_unlock(&sharedVariables.lock);

        // Incrémenter le tour du joueur, puis envoyer un signal à tous les threads qui attendent si le rendez-vous est atteint.
        pthread_mutex_lock(&shared->lock);
        shared->fillRate[args->teamNum] += water;
        printf("%sRemplissage actuel du seau : %d/%d\033[0m\n", color, shared->fillRate[args->teamNum], args->threshold);
        shared->playerTurn[args->teamNum] = (shared->playerTurn[args->teamNum] + 1) % args->lenPlayers;
        int canWakeThreads = 1;
        for (int i = 0; i < args->len; ++i) {
            if (shared->playerTurn[i] != shared->playerTurn[args->teamNum]) {
                canWakeThreads = 0;
            }
        }
        if (canWakeThreads) {
            pthread_cond_broadcast(&shared->cond);
        } else {
            pthread_cond_wait(&shared->cond, &shared->lock);
        }
        // Si quelqu'un a fini, on break 
        for (int i = 0; i < args->len; ++i) {
            if (shared->fillRate[i] >= args->threshold) {
                pthread_mutex_unlock(&shared->lock);
                // Dire aux enfants que c'est fini
                pthread_mutex_lock(&sharedVariables.lock);
                sharedVariables.currentPlayer = -1;
                pthread_cond_broadcast(&sharedVariables.cond);
                pthread_mutex_unlock(&sharedVariables.lock);
                break;
            }
        }
        pthread_mutex_unlock(&shared->lock);
    }

    // Clean exit : libération de la mémoire prise par les arguments.
    for (int i = 0; i < args->lenPlayers; ++i) {
        pthread_join(playerThreads[i], NULL);
    }

    pthread_cond_destroy(&sharedVariables.cond); pthread_mutex_destroy(&sharedVariables.lock); free(args);
    pthread_exit(NULL);
}

/**
 * Gère la logique du joueur :
 *  - while (1) : attend un signal de l'équipe.
 *  - si le tour du joueur actuel est -1, break.
 *  - sinon, si le tour du joueur actuel est le joueur, calcul + taux de remplissage aléatoire % (threshold / 10).
 **/
void *player(void *params) {
    PlayerParams *args = (PlayerParams *)params;
    PlayerSharedVariables *shared = args->shared;

    while (1) {
        pthread_mutex_lock(&shared->lock);
        pthread_cond_wait(&shared->cond, &shared->lock);
        if (shared->currentPlayer == -1) {
            pthread_mutex_unlock(&shared->lock);
            printf("%s[%d] Le joueur sort du terrain.\033[0m\n", args->color, args->playerNumber);
            break;
        } else if (shared->currentPlayer == args->playerNumber) {
            pthread_mutex_unlock(&shared->lock);
            int wait = /*(rand() % 5) +*/ 0; 
            printf("%s[%d] Course en %d secondes\033[0m\n", args->color, args->playerNumber, wait * 3);
            calcul(wait);
            int fill = (rand() % args->maxFill) + 1;
            printf("%s[%d] Remplissage du seau de %d cl.\033[0m\n", args->color, args->playerNumber, fill);
            // Broadcast le signal
            pthread_mutex_lock(&shared->lock);
            shared->water = fill;
            pthread_cond_broadcast(&shared->cond);
            pthread_mutex_unlock(&shared->lock);
        } else {
            pthread_mutex_unlock(&shared->lock);
        }
    }

    // Clean exit : libération des arguments.
    pthread_exit(NULL);
}