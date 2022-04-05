#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<string.h>
#define MSGSZ 128

typedef struct msgbuf{
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main(int argc, char **argv){
    int msqid;
    key_t key;
    message_buf rbuf;
    size_t len;

    if(argc != 2){
        (void)fprintf(stderr, "Usage : msgrecv key\n");
        exit(EXIT_FAILURE);
    }

    if((key = atoi(argv[1])) < 1){
        (void)fprintf(stderr, "Invalid key : %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if((msqid = msgget(key, 0)) < 0){
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if(msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0){
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    (void)printf("%s\n", rbuf.mtext);
    exit(EXIT_SUCCESS);

}
