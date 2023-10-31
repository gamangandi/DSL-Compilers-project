%{
#include<stdio.h>
#include<string.h>
FILE* tokens;
%}

W [0-9]+
number [1-9][0-9]*
id [a-zA-Z]([a-zA-Z0-9]|"_")*
strlit \"(\\.|[^"\\])*\"
charlit ['].*[']
int ("int")
char ("char")
string ("string")
bool ("bool")
file ("file")
void ("void")
pass ("pass")
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
mul "*"
sub "-"
div "/"
assign ("=")
ret ("return")
for ("for")
while ("while")
T ("true")
F ("false")
regex "regex"
grammar "grammar"
pointer ("- >"|"->")
reglabel "r"{number}
gramlabel "p"{number}
parse "parse"
open "open"


%%

^[ ]*\n {yylineno++;}

[ ]* { }

\n {yylineno++;}

{number} {fprintf(tokens, "constant: %s\n", yytext);}

{W} {fprintf(tokens, "constant 0: %s\n", yytext);}

{void} {fprintf(tokens, "Datatype: %s\n", yytext);}

{int} {fprintf(tokens,"Datatype: %s\n", yytext);}

{char} {fprintf(tokens,"Datatype: %s\n", yytext);}

{string} {fprintf(tokens,"Datatype: %s\n", yytext);}

{bool} {fprintf(tokens,"Datatype: %s\n", yytext);}

{file} {fprintf(tokens, "Datatype: %s\n", yytext);}

{pass} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{if} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{else} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{leq} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{geq} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{lt} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{gt} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{eq} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{ne} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{and} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{or} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{neg} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{add} {fprintf(tokens, "operator: %s\n", yytext);}

{mul} {fprintf(tokens, "operator: %s\n", yytext);}

{sub} {fprintf(tokens, "operator: %s\n", yytext);}

{div} {fprintf(tokens, "operator: %s\n", yytext);}

{colon} {fprintf(tokens, "punctuation: %s\n", yytext);}

{orsym} {fprintf(tokens, "special symbol: %s\n", yytext);}

{ques} {fprintf(tokens, "special symbol: %s\n", yytext);}

{semic} {fprintf(tokens,"Punctuation: %s\n", yytext);}

{comma} {fprintf(tokens,"Punctuation: %s\n", yytext);}

{openp} {fprintf(tokens, "special symbol: %s\n", yytext);}

{closep} {fprintf(tokens, "special symbol: %s\n", yytext);}

{openf} {fprintf(tokens, "special symbol: %s\n", yytext);}

{closef} {fprintf(tokens, "special symbol: %s\n", yytext);}

{ret} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{while} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{for} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{regex} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{grammar} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{assign} {fprintf(tokens, "operator: %s\n", yytext);}

{pointer} {fprintf(tokens, "operator: %s\n", yytext);}

{T} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{F} {fprintf(tokens, "reserved keyword: %s\n", yytext);}

{reglabel} {fprintf(tokens, "label: %s\n", yytext);}

{gramlabel} {fprintf(tokens, "label: %s\n", yytext);}

{open} {fprintf(tokens, "label: %s\n", yytext);}

{parse} {fprintf(tokens, "label: %s\n", yytext);}

{id} {fprintf(tokens, "id: %s\n", yytext);}

{strlit} {
    fprintf(tokens,"punctuation: \"\nconstant: ");
    int n = strlen(yytext);
    for(int i = 1; i < n-1; i++){
        if(yytext[i] != '\\'){
            fprintf(tokens,"%c",yytext[i]);
        }
    }
    fprintf(tokens,"\npunctuation: \"\n");
}
{charlit} {
    fprintf(tokens,"punctuation: \'\nconstant: ");
    if(strcmp(yytext,"\'\'") != 0){
        fprintf(tokens,"%c",yytext[1]);
    }
    fprintf(tokens,"\npunctuation: \'\n");
}
. {
    fprintf(tokens, "lexical error at line %d\n", yylineno);
    return 1;
}

%%
  
int yywrap(void){

}

int main(int argc, char* argv[]){   
    // The function that starts the analysis
    yyin = fopen(argv[1],"r");
    tokens = fopen("tokens.txt", "w");

    yylex();
    
    fclose(yyin);
    fclose(tokens);
  
    return 0;
}
