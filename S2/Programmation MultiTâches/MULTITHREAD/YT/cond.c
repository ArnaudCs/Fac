#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


void* hello1(void* data){
    char str[] = "Salut toi !";
    int i = 0;
    if(pthread_cond_wait(&cond, &mutex) !=0){
        perror("Erreur lors de l'attente de la condition\n");
        exit(1);
    } //attend une condition 
    while(i<strlen(str)){
        fprintf(stderr, "%c", str[i]);
        usleep(100000);
        i++;
    }
    fprintf(stderr, "\n");
    pthread_exit(NULL);
}

void* hello2(void* data){
    char str[] = "Aurevoir !";
    int i = 0;
    while(i<strlen(str)){
        fprintf(stderr, "%c", str[i]);
        usleep(100000);
        i++;
    }
    fprintf(stderr, "\n");
    pthread_cond_signal(&cond);
    pthread_exit(NULL);
}

int main(){
    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, hello1, NULL);
    pthread_create(&t2, NULL, hello2, NULL);
    if(pthread_join(t1, NULL) != 0){
        perror("Erreur lors de l'attente du thread\n");
        exit(1);
    }
    if(pthread_join(t2, NULL) != 0){
        perror("Erreur lors de l'attente du thread\n");
        exit(1);
    }

    pthread_cond_destroy(&cond);

    printf("Tous les threads ont terminés\n");

    return 0;
}