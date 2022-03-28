#include <stdlib.h>
#include <sys/types.h>
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>//perror
using namespace std;

// pour supprimer la file ayant la clé passée en parametre.
// une autre solution est d'utiliser la commande ipcrm -q <id_file>
// après avoir obtenue l'id de la file via la commande ipcs.

struct strMonMsg {
    long etiquette;
    int nom;
};

int main(int argc, char * argv[]){

  if (argc!=3) {
    cout<<"Nbre d'args invalide, utilisation :"<< endl;
    cout << argv[0] << " fichier-pour-cle-ipc entier-pour-cle-ipc"<< endl;
    exit(0);
  }
   
  key_t cle=ftok(argv[1], atoi(argv[2])); //identification de la clé, 

  if (cle==-1) {
    perror("Erreur ftok : ");
    exit(2);
  }

  cout << "ftok : validé" << endl;
    
  int msgid = msgget(cle, 0666);
  if(msgid==-1) {
    perror("(msgget) : Erreur récupération file : \n");
    exit(2);
  }
  
  cout << "msgget : validé" << endl;
  strMonMsg msg;
  msg.nom = 50;
  msg.etiquette = 10;

  int depot = msgsnd(msgid, &msg, sizeof(msg.nom), 0);
  if (depot == -1){
    perror("(msgrcv) : Erreur envoie de message :\n");
    exit(2);
  }
  else{
    cout << "Message envoy\n"<<endl;
  } 
  return 0;
}