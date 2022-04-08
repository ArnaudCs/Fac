#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include<stdlib.h>

#define limit_macane 50

struct equipe{
    int * tab_thread_joueur;
    int bassin ;
    pthread_mutex_t  bambou_lock; // bambou
    struct finish * fin ;
    int nb_joueur;
    int numero_equipe;
};

struct finish {
    int finish_val ; //sert a r 
    pthread_mutex_t * finish_lock; // bambou
    pthread_cond_t * finish_cond; // equipe attends que le sceau soit plein
};


void * fonctionEquipe (void * params){
    struct equipe * args = (struct equipe *) params;
    pthread_mutex_lock(&args->bambou_lock);
    for ( int i = 0; args->bassin < limit_macane ; i++){
        pthread_mutex_unlock(&args->bambou_lock);
        printf("Equipe %d Lancer du joueur :%d \n",args->numero_equipe,i%args->nb_joueur);
        sleep(2+args->numero_equipe);//:depart joueur i%args->nb_joueur
        pthread_mutex_lock(&args-> bambou_lock);
        args->bassin +=10;
        printf("Equipe %d  Fin du joueur :%d  bassin :%d \n",args->numero_equipe,i%args->nb_joueur,args->bassin);
    }

    pthread_mutex_unlock(&args-> bambou_lock);
    pthread_mutex_lock(args->fin->finish_lock);

    args->fin->finish_val=args->numero_equipe;

    pthread_cond_signal(args->fin->finish_cond); // on signal la cond changement sur le thread
    pthread_mutex_unlock(args->fin->finish_lock);
    pthread_exit(NULL);

}

int main(int argc, char * argv[]){

    if (argc < 3 ){
        printf("utilisation: %s  nombre_equipe nombre_joueur \n", argv[0]);
        return 1;
    }     
    int nb_equipe = 2;
    int nb_joueur = 4;
    
    pthread_t threads[nb_equipe]; //création des équipes avec les threads
    struct equipe tabParamas[nb_equipe]; //structs qui contient les infos sur les équipes

    int err = 0; 
    pthread_cond_t cond_finish; //création variable conditionnelle
    if((err = pthread_cond_init (&cond_finish,NULL))!=0){
        printf("Erreur init des conds \n");
        exit(1);
    } 
    pthread_mutex_t sh_lock ; //création du mutex
        if ((err = pthread_mutex_init (&sh_lock,NULL))!=0){
        printf("Erreur init du lock  \n");
        exit(1);
    }
        
    struct finish fin_equipe;
    fin_equipe.finish_val = 0;
    fin_equipe.finish_cond = &cond_finish;
    fin_equipe.finish_lock = &sh_lock; 
        
    for ( int i =0; i< nb_equipe ;i++){
        tabParamas[i].bassin = 0;
        if ((err= pthread_mutex_init (&tabParamas[i].bambou_lock,NULL))!=0){
            printf("Erreur init du lock  \n");
            exit(1);
        } 

        tabParamas[i].nb_joueur = nb_joueur;
        tabParamas[i].numero_equipe = i;
        tabParamas[i].tab_thread_joueur = (int *) malloc(sizeof(int)  * nb_joueur);
        tabParamas[i].tab_thread_joueur[k]=k;
        tabParamas[i].fin = &fin_equipe;
    }

    for ( int i =0; i< nb_equipe ;i++){ //création des threads
        printf("Equipe %d \n",i);
        if (pthread_create(&threads[i], NULL,(*fonctionEquipe),& tabParamas[i]) != 0){
            perror("erreur creation thread");
            exit(1);
        }
    }
                
    pthread_mutex_lock(&sh_lock);
    pthread_cond_wait(&cond_finish ,&sh_lock );
    printf("Equipe gagnante :%d \n" , fin_equipe.finish_val);

    return 0;

}