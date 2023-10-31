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
