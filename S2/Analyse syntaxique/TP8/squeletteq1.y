%{
#include <stdio.h>
#include <stdlib.h>s
#include "arbin.h"
void yyerror(char*);
int yylex();
%}
%union { /* YYSTYPE */
// TODO
}
// TODO : déclaration des tokens et des non terminaux
%%
ligne : error '\n' {yyerrok; exit(0);} /* error est un token fourni par bison et émit lors d'une erreur d'analyse  ; yyerrok signale que l'erreur a été traitée */
      | s '\n' {ab_afficher($1); exit(0);}
      ;
// TODO les règles !
%%s
int yylex(){
    int i=getchar();
    if ((i>='a' && i<='z')||i=='@'||i=='0'){
        yylval.typeInt=i;
        return SYMBOLE;
    }
    else return i;
}
void yyerror(char *s) {
    fprintf(stderr,"ERREUR : %s\n",s);
}
int main(){
    return yyparse();
}
