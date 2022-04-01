#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void * f1(){
    printf("Bonjour je suis le thread 1\n");
    pthread_exit(0);
}

void * f2(){
    printf("Bonjour je suis le thread 2\n");
    pthread_exit(0);
}

int main(){
    pthread_t thread1; //créer et nomme le thread
    pthread_t thread2;

    printf("Bonjour je suis le père\n");
    pthread_create(&(thread1), NULL, f1, NULL); //créer un thread en appelant la fonction 
    pthread_create(&(thread2), NULL, f2, NULL);
    pthread_join(thread1, NULL); //attend la fin du thread avant la fin du programme
    pthread_join(thread2, NULL);

    return EXIT_SUCCESS;
}