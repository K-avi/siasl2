/* based on the brainflex lex.l file https://github.com/lagerfeuer/brainflex/blob/main/src/lexer.l */

%option header-file="lex.yy.h"
%option noinput nounput noyywrap


%{
  #include "parser.tab.h"
  #define TOKEN(t) yylval.token = (t)
%}

%%

[ \t\n]+    { ; }
"."         { return TOKEN(PRINT); }
","         { return TOKEN(READ); }
">"         { return TOKEN(RIGHT); }
"<"         { return TOKEN(LEFT); }
"^"         { return TOKEN(UP); }
"v"         { return TOKEN(DOWN); }
"+"         { return TOKEN(PLUS); }
"-"         { return TOKEN(MINUS); }
"*"         { return TOKEN(MULT);}
"/"         { return TOKEN(DIV);}
"["         { return TOKEN(LBRACKET); }
"]"         { return TOKEN(RBRACKET); }
"#"         { return TOKEN(NEUTRAL); }
"("         { return TOKEN(NEUTRAL); }
")"         { return TOKEN(NEUTRAL); }
"?"         { return TOKEN(NEUTRAL); }

.           { ; }

%%