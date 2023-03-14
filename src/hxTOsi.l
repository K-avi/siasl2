/* based on the brainflex lex.l file https://github.com/lagerfeuer/brainflex/blob/main/src/lexer.l */

%option header-file="lex.hh.h"
%option noinput nounput noyywrap

%option prefix="hh"

%{
  	#include <stdio.h>
	#include <stdlib.h>
%}

%%

[ \t\n]+    { ; }
"0"			{fprintf(hhout, "#");}
"1"			{fprintf(hhout, "#");}
"2"			{fprintf(hhout, "<");}
"3"			{fprintf(hhout, ">");}
"4"			{fprintf(hhout, "^");}
"5"			{fprintf(hhout, "v");}
"6"			{fprintf(hhout, "+");}
"7"			{fprintf(hhout, "-");}
"8"			{fprintf(hhout, "*");}
"9"			{fprintf(hhout, "/");}
"a"|"A"		{fprintf(hhout, ",");}
"b"|"B"		{fprintf(hhout, ".");}
"c"|"C"		{fprintf(hhout, "[");}
"d"|"D"		{fprintf(hhout, "]");}
"e"|"E"		{fprintf(hhout, "#");}
"f"|"F"		{fprintf(hhout, "#");}
.           { ; }

%%


