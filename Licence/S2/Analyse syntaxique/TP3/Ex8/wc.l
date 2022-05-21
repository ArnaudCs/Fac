%{ /* analflex.l */
/* ZONE DE DEFINITION (OPTIONNELLE) */
int nbcar=0;
int nbmot=0;
int nbligne=0;
%}
%%
[^\n\t ]+ {nbmot++; nbcar+=yyleng;}
. {nbcar++;}
\n {nbcar++;nbligne++;}
%%
/* ZONE DES FONCTIONS C */
int main(int nbarg, char* argv[]) 
{
 if(nbarg==2)
  {yyin=fopen(argv[1], "r");}
 yylex();
 printf("\nnbcar : %d ; nbmot : %d ; nbligne : %d\n", nbcar, nbmot, nbligne);
 printf("\n");
}
