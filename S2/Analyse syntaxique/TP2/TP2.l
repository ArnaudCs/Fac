%{ /* analflex.l */
/* ZONE DE DEFINITION (OPTIONNELLE) */
/* ZONE DES REGLES apres le double pourcent (OBLIGATOIRE) */
%}
%%
if {return 300; /* ret un jeton */ }
then {return 301; /* ret un jeton */ }
else {return 302; /* ret un jeton */}
throw {return 303; /* ret un jeton */}
(" "|"\t"|"\n"|"\r"|"\f"|"//".*\n|("/*"(.|\n)*"*/")) {return 304;}
0[0-7]* {return 305;}
[1-9][0-9]* {return 306; /* ret un jeton */}
0x[0-9A-Fa-f]* {return 306;}
[a-zA-Z"_"][A-Za-z"_"0-9]* {return 307; /* ret un jeton */}
([0-9]+\.[0-9]*|\.[0-9]+)([EeDd][+-]?[0-9]+)? {return 308; /* ret un jeton */}
.*|\n {return -1; /* ret un jeton pour tout le reste */ }
%%45
/* ZONE DES FONCTIONS C */
main()
{int j; 
char *invite="Saisissez un(des) mot(s) SVP :";
printf(invite);
while ((j=yylex())!=0) printf("\nJeton : %i; de lexeme %s\n%s",j,yytext,invite);}
