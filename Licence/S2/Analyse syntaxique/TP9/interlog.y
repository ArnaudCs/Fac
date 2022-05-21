%{				// interlog.y  
#include  <stdio.h>
#define YYSTYPE int             /* inutile */
int yylex(void);  void yyerror(char *s);
%}
%%
liste   :                 {}
        | liste ligne     {}
        ;
ligne   : expr '\n'       {printf("\nRésultat : %d\n",$1);}
        ;
expr    : '(' expr ')'    {$$ = $2;}
        | expr '|' expr   {$$ = $1 || $3;}
        | expr '&' expr   {$$ = $1 && $3;}
        | '!' expr        {$$ = ! $2;}
	      |	'0'             {$$ = 0;}
        | '1'             {$$ = 1;}
        ;
%%
int yylex(void) {
  int c;
  while(((c=getchar())==' ') || (c=='\t'))
    ;
  return c;
}
void yyerror(char *s) {
  fprintf(stderr,"%s\n",s);
}
int main(void){
  printf("Veuillez entrer une expression booléenne S.V.P. : ");
  return yyparse();
}
