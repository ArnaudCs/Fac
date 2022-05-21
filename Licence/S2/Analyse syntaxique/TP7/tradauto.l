%{
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "arbin.h"

// | < concaténation < * < ()
// [a-z], @ : epsilon, 0 : mot vide

/*
ligne -> S'\n'
S -> E X
X -> '|' E X | @
E -> T R
R -> E | @
T -> F Y
Y -> * Y | @
F -> '(' E ')' | BASE

BASE = [a-z0@]
*/

struct YYData {
    char *name;
    int val;
};
struct YYData yydata;

#define yymkdata(value) \
    yydata.val = value; yydata.name = #value; return value;

enum Token {
    LITFLOT = 300,
};

double yylval;
%}
%option noyywrap
%%
[a-z0@]*  { yylval = atof(yytext); yymkdata(LITFLOT) }
(.|\n)                                                          { return yytext[0]; }
%%

#define AVANCER {jeton=getchar();numcar++;}
#define TEST_AVANCE(prevu) {if (jeton==(prevu)) AVANCER else ERREUR_SYNTAXE}
#define ERREUR_SYNTAXE {printf("\nMot non reconnu : erreur de syntaxe \
au caractère numéro %d, de jeton %d \n",numcar,jeton); exit(1);}
#define INVITE "Veuillez saisir une expression numérique SVP (q pour quitter) : "

Arbin S(); Arbin X(Arbin g); Arbin E(); Arbin R(Arbin g); Arbin T(); Arbin Y(Arbin g); Arbin F(); void ligne();
int jeton;                                  /* caractère courant du flot d'entrée */
int numcar = 0;                             /* numero du caractère courant (jeton) */

Arbin S() {
    return X(E());
}
Arbin X(Arbin g) {
            ab_afficher(g);

    if (jeton == '|') {
        AVANCER
        return X(ab_construire('|',g,E()));
    }
    else {
        return g;
    }
}
Arbin E() {
    return R(T());
}
Arbin R(Arbin g) {
    if (jeton == '0' || jeton =='@' || islower(jeton) || jeton == '(') {
        return R(ab_construire('.',g,T()));
    }
    else {
        return g;
    }
}
Arbin T() {
    return Y(F());
}
Arbin Y(Arbin g) {
    if (jeton == '*') {
        AVANCER
        return Y(ab_construire('*',g,NULL));
    }
    else {
        return g;
    }
}
Arbin F() {
    if (jeton == '(') {
        AVANCER
        Arbin a = E();
        TEST_AVANCE(')');
        return a;
    }
    else {
        if (jeton == '0' || jeton =='@' || islower(jeton)) {
            Arbin a = ab_construire(jeton,NULL,NULL);
            AVANCER
            return a;
        }
        else {
            ERREUR_SYNTAXE
        }
    }
}

void ligne() {
    Arbin a = S();
    TEST_AVANCE('\n')
    printf("Arbre binaire associé :\n");
    ab_afficher(a);
}

int main(){
    printf(INVITE);
    numcar=0;
    AVANCER;
    ligne();
    return 0; /* ne retourne jamais que par X */ 
}
