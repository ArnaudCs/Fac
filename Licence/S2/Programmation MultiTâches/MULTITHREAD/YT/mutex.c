#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* hello1(void* data){
    char str[] = "Salut toi !";
    int i = 0;
    pthread_mutex_lock(&mutex);
    while(i<strlen(str)){
        fprintf(stderr, "%c", str[i]);
        usleep(100000);
        i++;
    }
    fprintf(stderr, "\n");
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* hello2(void* data){
    char str[] = "Aurevoir !";
    int i = 0;
    pthread_mutex_lock(&mutex);
    while(i<strlen(str)){
        fprintf(stderr, "%c", str[i]);
        usleep(100000);
        i++;
    }
    fprintf(stderr, "\n");
    pthread_mutex_unlock(&mutex);
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
    pthread_mutex_destroy(&mutex);

    printf("Tous les threads ont terminés\n");

    return 0;
}