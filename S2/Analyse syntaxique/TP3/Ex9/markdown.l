%{ /* analflex.l */
#include <stdio.h>
int niveau = 0;
int tab[10];
%}
%option noyywrap;
%s DANSLISTE;
%%
^\n[ \t]*[-*] {BEGIN(DANSLISTE);
	       niveau = 0;
	       tab[niveau] = yyleng-2; //nb de blancs au début (2 pour \n et *)
	       puts("<ul><li>");}

<DANSLISTE>^[ \t]*[*-] { puts("</li>");
		         int diff = (yyleng -1) - tab[niveau]; //1 pour *
		         if(diff == 0 || diff == 1) { //même niveau même si 1 blanc en plus
		           puts("<li>");}
    		         else if (diff >=2) {
			   puts("<ul><li>");
			   niveau++;
			   tab[niveau] = yyleng-1;} //1 pour *
		       	 else {
			  do{ 
			   puts("</li>"); //fermer 
			   niveau--;}
	                  while(niveau>0 && tab[niveau]>yyleng-1);
			 puts("<li>");
			 }
			}

<DANSLISTE>^[ \t]*\n { BEGIN(0); } //??????????????????????????????????

"**"[^*\n]+"**" {
	puts("<b>");
	fwrite(yytext+2, yyleng-4, 1 stdout);
	puts("</b>");
}

^"#"{1, 6}.+\n{
	int i = 0;
	for(;yytext[i] == '#'; i++); //compter les #
	printf("<h%c>", '0'+i);
	fwrite(yytext+i, yyleng-i-1, 1, stdout);
	printf("</h%c>\n", '0'+i);
}
.|\n {printf("%c", yytext[0]);

%%
/* ZONE DES FONCTIONS C */
int main() {int i; while((i=yylex()));}
