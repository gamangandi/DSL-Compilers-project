%{
#include<stdio.h>
#include<string.h>
#include "parser.tab.h"
FILE* tokens;
FILE* parsed;
extern int flowers;

%}

W [0-9]+
number [1-9][0-9]*
id [a-zA-Z]([a-zA-Z0-9]|"_")*
strlit \"(\\.|[^"\\])*\"
charlit ['].*[']
int ("int")
char ("char")
string ("string")
file "file"
bool ("bool")
void ("void")
pass "pass"
if "if"
else "else"
gt ("gt")
lt ("lt")
leq ("leq")
geq ("geq")
eq ("eq")
ne ("ne")
and ("and")
or ("or")
neg ("neg")
openp ("(")
closep (")")
openf ("{")
closef ("}")
semic (";")
comma (",")
colon ":"
orsym "|"
ques "?"
add "+"
sub "-"
mul "*"
div "/"


%%

^[ ]*\n {yylineno++;}

[ ] {fprintf(parsed, " ");}

[ ]* { }

\n {yylineno++; fprintf(parsed, "\n");}

{number} {fprintf(parsed, "%s", yytext); fprintf(tokens, "constant: %s\n", yytext); return num;}

{W} {fprintf(parsed, "%s", yytext); fprintf(tokens, "constant 0: %s\n", yytext); return W;}

{void} {fprintf(parsed, "%s", yytext); fprintf(tokens, "reserved keyword: %s\n", yytext); return Void;}

{int} {fprintf(parsed, "%s", yytext); fprintf(tokens,"Datatype: %s\n", yytext); return intd;}

{char} {fprintf(parsed, "%s", yytext); fprintf(tokens,"Datatype: %s\n", yytext); return chard;}

{string} {fprintf(parsed, "%s", yytext); fprintf(tokens,"Datatype: %s\n", yytext); return stringd;}

{file} {fprintf(parsed, "%s", yytext); fprintf(tokens, "Datatype: %s\n", yytext); return filed;}

{bool} {fprintf(parsed, "%s", yytext); fprintf(tokens,"Datatype: %s\n", yytext); return boold;}

{pass} {fprintf(parsed, "%s", yytext); fprintf(tokens, "reserved keyword: %s\n", yytext); return pass;}

{if} {fprintf(parsed, "%s", yytext); fprintf(tokens, "reserved keyword: %s\n", yytext); return If;}

{else} {fprintf(parsed, "%s", yytext); fprintf(tokens, "reserved keyword: %s\n", yytext); return Else;}

{leq} {fprintf(parsed, "%s", yytext); fprintf(tokens, "reserved keyword: %s\n", yytext); return leq;}

{geq} {fprintf(parsed, "%s", yytext); fprintf(tokens, "reserved keyword: %s\n", yytext); return geq;}

{lt} {fprintf(parsed, "%s", yytext); fprintf(tokens, "reserved keyword: %s\n", yytext); return lt;}

{gt} {fprintf(parsed, "%s", yytext); fprintf(tokens, "reserved keyword: %s\n", yytext); return gt;}

{eq} {fprintf(parsed, "%s", yytext); fprintf(tokens, "reserved keyword: %s\n", yytext); return eq;}

{ne} {fprintf(parsed, "%s", yytext); fprintf(tokens, "reserved keyword: %s\n", yytext); return ne;}

{and} {fprintf(parsed, "%s", yytext); fprintf(tokens, "reserved keyword: %s\n", yytext); return and;}

{or} {fprintf(parsed, "%s", yytext); fprintf(tokens, "reserved keyword: %s\n", yytext); return or;}

{neg} {fprintf(parsed, "%s", yytext); fprintf(tokens, "reserved keyword: %s\n", yytext); return neg;}

{add} {fprintf(parsed, "%s", yytext); fprintf(tokens, "operator: %s\n", yytext); return add;}

{mul} {fprintf(parsed, "%s", yytext); fprintf(tokens, "operator: %s\n", yytext); return mul;}

{sub} {fprintf(parsed, "%s", yytext); fprintf(tokens, "operator: %s\n", yytext); return sub;}

{div} {fprintf(parsed, "%s", yytext); fprintf(tokens, "operator: %s\n", yytext); return divi;}

{semic} {fprintf(parsed, "%s", yytext); fprintf(tokens,"Punctuation: %s\n", yytext); return semic;}

{openp} {fprintf(parsed, "%s", yytext); fprintf(tokens, "special symbol: %s\n", yytext); return openp;}

{closep} {fprintf(parsed, "%s", yytext); fprintf(tokens, "special symbol: %s\n", yytext); return closep;}

{openf} {fprintf(parsed, "%s", yytext); flowers++; fprintf(tokens, "special symbol: %s\n", yytext); return openf;}

{closef} {fprintf(parsed, "%s", yytext); flowers--; fprintf(tokens, "special symbol: %s\n", yytext); return closef;}

{colon} {fprintf(parsed, "%s", yytext); fprintf(tokens, "punctuation: %s\n", yytext); return colon;}

{orsym} {fprintf(parsed, "%s", yytext); fprintf(tokens, "special symbol: %s\n", yytext); return orsym;}

{ques} {fprintf(parsed, "%s", yytext); fprintf(tokens, "special symbol: %s\n", yytext); return ques;}

{comma} {fprintf(parsed, "%s", yytext); fprintf(tokens,"Punctuation: %s\n", yytext); return comma;}




{id} {fprintf(parsed, "%s", yytext); fprintf(tokens, "id: %s\n", yytext); return id;}

{strlit} {
    fprintf(tokens,"punctuation: \"\nconstant: ");
    int n = strlen(yytext);
    for(int i = 1; i < n-1; i++){
        if(yytext[i] != '\\'){
            fprintf(tokens,"%c",yytext[i]);
        }
    }
    fprintf(tokens,"\npunctuation: \"\n");
    fprintf(parsed, "%s", yytext);

    return strlit;
}
{charlit} {
    fprintf(tokens,"punctuation: \'\nconstant: ");
    if(strcmp(yytext,"\'\'") != 0){
        fprintf(tokens,"%c",yytext[1]);
    }
    fprintf(tokens,"\npunctuation: \'\n");
    fprintf(parsed, "%s", yytext);

    return charlit;
}
. {
    fprintf(tokens, "error at line %d\n", yylineno);
    return 1;
}

%%
  
int yywrap(void){

}
