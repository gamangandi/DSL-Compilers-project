#!/bin/bash
lex lexer_for_parser.l
bison -t -d -Wcounterexample parser.y
gcc lex.yy.c parser.tab.c
./a.out input.txt #input file name should be changed as per the need
