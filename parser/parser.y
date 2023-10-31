%{
#include <stdio.h>

int yylex();
int yyerror(char *s);
extern FILE* parsed;
extern FILE* yyin;
extern FILE* tokens;

int flowers = 0;
extern int yylineno;

int rets = 0;

%}

%token num W If Else task leq geq lt  gt eq ne and or add mul sub divi ret While For Void intd chard stringd filed boold semic open parse comma openp closep closef openf assign pointer T F neg  reglabel gramlabel colon orsym ques regex grammar pass id strlit charlit

%%

%%
int yyerror(char *s)
{
	fprintf(parsed,"\n%s at line %d\n", s, yylineno);
   printf("%s at line %d\n", s, yylineno);
	return 0;
}

int main(int argc, char* argv[]){

      /* #ifdef YYDEBUG
      yydebug = 1;
      #endif */

	yyin = fopen(argv[1], "r");
	tokens = fopen("seq_tokens_.txt", "w");
   parsed = fopen("output.txt", "w");
	
	yyparse();
	
	fclose(yyin);
   fclose(parsed);
	fclose(tokens);
  
   return 0;
}
