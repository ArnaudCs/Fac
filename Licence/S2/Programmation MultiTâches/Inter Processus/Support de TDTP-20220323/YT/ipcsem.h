#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

void creersem(key_t cle, int N);
void initsem(int N, int V);
void P(int N);
void V(int N);
void detruiresem();
void attente (int N);