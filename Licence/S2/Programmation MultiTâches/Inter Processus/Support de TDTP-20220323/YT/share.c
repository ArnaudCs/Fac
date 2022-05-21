#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>
#include<err.h>
#include<errno.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/msg.h>
#include<string.h>

#define SHM_ID     42
#define SHM_SIZE 1024

int main(int argc, char** argv){
    key_t key;
    int shmid;
    char *data;

    if(argc > 2){
        (void)fprintf(stderr, "Usage : shmdemo [data_à_écrire]");
        exit(EXIT_FAILURE);
    }

    if((key = ftok("./share.c", SHM_ID)) == -1){
        err(EXIT_FAILURE, "ftok");
    }

    if((shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT)) == -1){
        err(EXIT_FAILURE, "shmget");
    }

    data = shmat(shmid, (void *)0, 0);
    if(data == (char *)(-1)){
        err(EXIT_FAILURE, "shmat");
    }

    if(argc == 2){
        (void)printf("%d : writing to segment : \"%s\"\n", getpid(), argv[1]);
        (void)strncpy(data, argv[1], SHM_SIZE);
    }else
        (void)printf("%d: segment contains : \"%s\"\n", getpid(), data);

    if(shmdt(data) == -1){
        err(EXIT_FAILURE, "shmdt");
    }

    return EXIT_SUCCESS;
}