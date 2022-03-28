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

struct sMsg {
  long etiquette;
  int mot;
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
    
  int msgid = msgget(cle, IPC_CREAT|IPC_EXCL| 0666);
  if(msgid==-1) {
    perror("(msgget) : Erreur récupération file : \n");
    exit(2);
  }
  
  cout << "msgget : validé" << endl;
  sMsg msg;

  int retour = msgrcv(msgid, &msg, (size_t)sizeof(msg.mot), (long)msg.etiquette, 0);
  if (retour == -1){
    perror("(msgrcv) : Erreur reception du message :\n");
    exit(2);
  }
  else{
    cout << "Message reçu : "<<msg.mot<<endl;
  } 
  return 0;
}
