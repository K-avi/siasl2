/* based on brainflex grammar.y file https://github.com/lagerfeuer/brainflex/blob/main/src/grammar.y */
%{
  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include <ctype.h>
  #include <unistd.h>

  #include "ast.h"

  extern FILE* yyin;
  extern int yylex(void);
  extern int yyparse(void);
  void yyerror(const char *s);

  struct instruction * prog;
%}

%union {
  struct instruction* instruction;
  int token;
}

%token <token> PRINT READ LEFT RIGHT UP DOWN PLUS MINUS MULT DIV NEUTRAL WILDCARD
%token <token> LBRACKET RBRACKET

%type <instruction> program stmts stmt loop
%type <token> syllable
%type <symbol> token token 

%start program

%%

program
  : stmts { prog = $$; }
;

stmts
  : stmt        { $$ = $1; }
  | stmts stmt  { add_instruction($$ = $1, $2); }
;

stmt
  : loop { $$ = $1; }
  | symbol   { $$ = mkinstruction($1, $2); }
;

loop
  : LBRACKET stmts RBRACKET {
      $$ = mkinstruction($1);
      $$->next = $2;
      $$->other = mkinstruction($3);
      add_instruction($$->next, $$->other);
      $$->other->other = $$;
    }
;

syllable
  : PRINT
  | READ
  | LEFT
  | RIGHT
  | UP
  | DOWN
  | PLUS
  | MINUS
  | MULT
  | DIV
  | NEUTRAL
  | WILDCARD
  | LBRACKET 
  | RBRACKET
;

%%


void yyerror(const char* s) {
  fprintf(stderr, "yyerr: %s\n", s);
}
