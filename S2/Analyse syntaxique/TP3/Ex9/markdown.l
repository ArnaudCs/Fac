%{ /* analflex.l */
/* ZONE DE DEFINITION (OPTIONNELLE) */
/* ZONE DES REGLES apres le double pourcent (OBLIGATOIRE) */
%}
%%
^[*] {int c;
	if ((c=input())!=EOF && c!=0){
	//printf("<CHAR %c (%d)>", c,c); // débogage
	unput(c); //rejet du char diff de EOF
	putchar('<ul><li>'); /* à changer en espace */
	}
^[\t ]{2,} {int c;
	if ((c=input())!=EOF && c!=0){
	//printf("<CHAR %c (%d)>", c,c); // débogage
	unput(c); //rejet du char diff de EOF
	putchar('</li><ul><li>'); /* à changer en espace */
	}
}

%%
/* ZONE DES FONCTIONS C */
int main() {int i; while((i=yylex()));}
