#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
                             /* les macros sont des blocs : pas de ';' apres */
#define AVANCER {jeton=getchar();numcar++;}
#define TEST_AVANCE(prevu) {if (jeton==(prevu)) AVANCER else ERREUR_SYNTAXE}
#define ERREUR_SYNTAXE {printf("\nMot non reconnu : erreur de syntaxe \
au caractère numéro %d \n",numcar); exit(1);} 

int E();int R(int x);int T();int S(int x);int F(int x); /* déclars */

int jeton;                       /* caractère courant du flot d'entrée */
int numcar=0;                    /* numero du caractère courant (jeton) */
int somme = 0;

int E(int x){
  R(T());                          /* regle : E->TR */
}

int R(int x){
  if (jeton=='+') {             /* regle : R->+TR */
    AVANCER
    T();
    R();
  }
  else return x;                /* regle : R->epsilon */
}

int T(int x){
  S(F());                          /* regle : T->FS */
  return T(x) + R(x);
}

int S(int x){
  if (jeton=='*') {             /* regle : S->*FS */
    AVANCER
    F();
    S();
    return F(x) * S(x);
  }
  else ;                        /* regle : S->epsilon */
}

int F(int x){
  if (jeton=='(') {             /* regle : F->(E) */
    AVANCER
    E();
    TEST_AVANCE(')')
  }
  else 
    if (isdigit(jeton))         /* regle : F->0|1|...|9 */
      AVANCER
    else ERREUR_SYNTAXE
}

int main(void){                 /* Fonction principale */
  AVANCER			/* initialiser jeton sur le premier car */
  E();                          /* axiome */
  if (jeton=="\n")               /* expression reconnue et rien après */
    printf("\nMot reconnu\n"); 
  else ERREUR_SYNTAXE           /* expression reconnue mais il reste des car */

  return 0;
}
