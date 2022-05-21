#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>
#include<err.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<string.h>

#define MAX_RETRIES 10

int main(void){
    key_t key;
    int semid;
    struct sembuf sb = {
        .sem_num = 0, 
        .sem_op = SEMLOCK,
        .sem_flg = SEM_UNDO,
    };

    if((key = ftok("./semdemo.c", SEMID)) == -1){
        err(EXIT_FAILURE, "ftok");
    }

    if((semid = initsem(key)) == -1){
        err(EXIT_FAILURE, "initsem");
    }

    (void)printf("Press enter to lock : ");
    (void)getchar();
    (void)printf("Trying to lock ...");

    if(semop(semid, &sb, 1) == -1){
        err(EXIT_FAILURE, "semop");
    }

    (void)printf("Locked");
    (void)printf("Press enter to unlock");
    (void)getchar();

    sb.sem_op = SEMUNLOCK;
    if(semop(semid, &sb, 1) == -1){
        err(EXIT_FAILURE, "semop");
    }

    (void)printf("Unlocked.\n");
    return EXIT_SUCCESS;

}