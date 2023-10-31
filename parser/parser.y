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

S : Reg S | Gram S | M S |;

M : func_decl MethodBody {if(rets == 0){yyerror("return error"); return 1;} else if(rets != 0){rets = 0;}};

func_decl : types id arglist{fprintf(parsed, " : function declaration");};

types : intd | chard | stringd | boold | Void | filed;

arglist : openp X closep | openp closep;

X : types id X1;

X1 : 
   |comma X;

MethodBody : openf stmts closef;

stmts : 
      | stmt stmts
      | MethodBody stmts;

stmt  : istmt 
	  | lstmt 
	  | dstmt
	  | estmtnotfor
	  | cstmt
	  | rstmt;

rstmt : ret retarg semic{fprintf(parsed, " : return statement"); rets++;};

retarg :
       | Void
       | Phelper;


istmt : If openp P closep {fprintf(parsed, " : conditional statement");} MethodBody E;

negex : neg openp neghelper closep

neghelper : Phelper;

P : Phelper | ;

Phelper : A logic Phelper | A;

boolean : T | F;

logic : geq| leq | gt | lt | ne | eq | and | or;

A : id
  | num
  | W
  | charlit
  | strlit
  | boolean
  | binops
  | unops
  | negex
  | callees
  | filerhs
  | openp Phelper closep;

E : 
  | Else {fprintf(parsed, " : conditional statement");} MethodBody;

lstmt : forstmt | whilestmt;

forstmt : for_decl MethodBody;

for_decl : For openp estmt P semic forhelper closep {fprintf(parsed, " : loop");};

estmtnotfor : estmt{fprintf(parsed, " : expression statement");}; 

forhelper : | unops;

whilestmt : while_decl MethodBody;

while_decl : While openp P closep {fprintf(parsed, " : loop");} task; 


dstmt : types Y dhelp semic{fprintf(parsed, " : declaration statement");};

dhelp : assign exprhs | ;

Y : id Y1;

Y1 :
   | comma Y;

estmt : id assign exprhs semic;

exprhs : Phelper;

binop : add | mul | sub | divi

binops : openp bh binop bh closep;

unops : bh unophelp;
unophelp : add add | sub sub;

bh : id
   | num
   | W
   | boolean
   | binops
   | callees
   | openp Phelper closep;


cstmt : ch;

ch : callees semic{fprintf(parsed, " : call statement");};

callees : id callarglist | parsecall;

callarglist : openp Args closep;

Args : J Args1;

Args1 :
   | comma Args;

J : Phelper;




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
