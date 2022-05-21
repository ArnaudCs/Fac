/** 
 * @file analdesc.c        
 * @author Michel Meynard
 * @brief Analyse descendante récursive d'expression arithmétique
 *
 * Ce fichier contient un reconnaisseur d'expressions arithmétiques composée de 
 * littéraux entiers sur un car, des opérateurs +, * et du parenthésage ().
 * Remarque : soit rediriger en entrée un fichier, soit terminer par deux 
 * caractères EOF (Ctrl-D), un pour lancer la lecture, l'autre comme "vrai" EOF.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "arbin.h"

#define AVANCER { jeton = getchar(); numcar++; }
#define TEST_AVANCE(prevu) { if (jeton == (prevu)) AVANCER else ERREUR_SYNTAXE }
#define ERREUR_SYNTAXE { printf("\nMot non reconnu : erreur de syntaxe \
au caractère numéro %d \n",numcar); exit(1); } 

/*GREFFERGAUCHE*/
void ab_greffergauche(Arbin pere, Arbin filsg) {
    ab_vider(&ab_sag(pere));		/* vider l'ancien sag */
    pere->fg=filsg;		/* le remplacer */
}

/*GREFFERDROITE*/
void ab_grefferdroite(Arbin pere, Arbin filsd){
    ab_vider(&ab_sad(pere));
    pere->fd=filsd;
}

/*CONSTRUIRE*/
Arbin ab_construire(int rac, Arbin g, Arbin d){
    Arbin nouv = (Arbin) malloc(sizeof(Noeudbin)) ;
    nouv->val=rac;
    nouv->fg=g;
    nouv->fd=d;
    return(nouv) ;
}

/*COPIERA*/
Arbin ab_copier(Arbin a){
    Arbin nouv;			/* nouvel arbin copie */
    if (ab_vide(a)) return NULL;	/* arbin vide */
    nouv = (Arbin) malloc(sizeof(Noeudbin)) ;
    nouv->val=a->val;
    nouv->fg=ab_copier(ab_sag(a));
    nouv->fd=ab_copier(ab_sad(a));
    return(nouv) ;
} 

/*VIDERA*/
void ab_vider(Arbin *pa){	
    Arbin *pg,*pd;			/* arbins de sauvegarde */
    if (ab_vide(*pa)) return;		/* arbin vide */
    pg=&(ab_sag(*pa));			/* possible car sag() est une macro */
    pd=&(ab_sad(*pa));
    ab_vider(pg);
    ab_vider(pd);
    free(*pa);			/* on est sur une feuille */
    *pa=NULL;			/* mettre le pointeur a NULL  */
    return;
} 
/*AFFICHERAR-fonction privée (static)*/
static void ab_afficherR(Arbin a,int indent){
    int i;
    for (i=0;i<indent;i++)
        printf("    ");		/* 4 espaces par profondeur d'indentation */
    if (ab_vide(a))
        printf("\r");
    else {
        printf("%c\n",ab_racine(a));
        ab_afficherR(ab_sag(a),indent+1);
        ab_afficherR(ab_sad(a),indent+1);
  }
}
/*AFFICHERA*/
void ab_afficher(Arbin a){
    ab_afficherR(a,0);
}

Arbin E(); Arbin R(); Arbin T(); Arbin S(); Arbin F(); 

int jeton;                                  /* caractère courant du flot d'entrée */
int numcar = 0;                             /* numero du caractère courant (jeton) */

Arbin E() {                                 /* regle : E->TR */
    Arbin t = T();
    Arbin r = R();
    // R() a généré epsilon, on ne prend que le sous-arbre T().
    if (ab_vide(r)) {
        return t;
    }
    // R() est une opération :
    //  1 - On prend sa racine et son sag et on fait un sous-arbre contenant l'opération 
    //  2 - Le sous-arbre droit devient le parent :
    //      * Transfert de la racine du sad en tant que racine de l'arbre
    //      * On récupère le fils non nul du sad pour le placer en sad.
    Arbin op = ab_construire(ab_racine(r), t, ab_sag(r));
    if (ab_vide(ab_sad(r))) {
        return op;
    }
    return ab_construire(ab_racine(ab_sad(r)), op, ab_sag(ab_sad(r))); 
}

Arbin R() {                                  /* regle : R->+TR|epsilon */
    Arbin rac = ab_creer();
    if (jeton == '+') {                     
        AVANCER
        Arbin t = T();
        Arbin r = R();
        rac = ab_construire('+', t, r);
    }
    return rac;
}

Arbin T() {                                  /* regle : T->FS */
    Arbin f = F();
    Arbin s = S();
    if (ab_vide(s)) {
        return f;
    }
    Arbin op = ab_construire(ab_racine(s), f, ab_sag(s));
    if (ab_vide(ab_sad(s))) {
        return op;
    }
    return ab_construire(ab_racine(ab_sad(s)), op, ab_sag(ab_sad(s))); 
}

Arbin S() {                                  /* regle : S->*FS|epsilon */
    Arbin rac = ab_creer();
    if (jeton == '*') {                     
        AVANCER
        Arbin f = F();
        Arbin s = S();
        rac = ab_construire('*', f, s);
    }
    return rac;
}

Arbin F() {                                  /* regle : F->(E)|0|1|...|9 */
    if (jeton == '(') {                     
        AVANCER
        Arbin e = E();
        TEST_AVANCE(')')
        return e;
    }
    else {
        if (isdigit(jeton)) {
            int val = jeton;
            AVANCER
            return ab_construire(val, ab_creer(), ab_creer());
        }
        else {
            ERREUR_SYNTAXE
        }
    }
}

int main() {         
    printf("Entrez la formule à transformer : ");                    
    AVANCER			                        /* initialiser jeton sur le premier car */
    Arbin abr = E();                        /* axiome */
    if (jeton == '\n') {                    /* expression reconnue et rien après */
        printf("\nMot reconnu.\n"); 
        ab_afficher(abr); 
    }
    else {
        ERREUR_SYNTAXE                      /* expression reconnue mais il reste des car */
    }
    ab_vider(&abr);
    return 0;
}