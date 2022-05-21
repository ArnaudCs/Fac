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
    message_buf sbuf;
    size_t len;

    if(argc != 3){
        (void)fprintf(stderr, "Usage : msgsend key message\n");
        exit(EXIT_FAILURE);
    }

    if((key = atoi(argv[1])) < 1){
        (void)fprintf(stderr, "Invalid key : %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if((msqid = msgget(key, IPC_CREAT | 0666)) < 0){
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    sbuf.mtype = 1;
    (void)strncpy(sbuf.mtext, argv[2], MSGSZ);
    len = strlen(sbuf.mtext) + 1;

    if(msgsnd(msqid, &sbuf, len, IPC_NOWAIT) < 0){
        (void)fprintf(stderr, "%d, %ld, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, (int)len);
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}