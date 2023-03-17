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

  struct symbol{ int token1; int token2; } sym;

}

%token <token> PRINT READ LEFT RIGHT UP DOWN PLUS MINUS MULT DIV NEUTRAL WILDCARD
%token <token> LBRACKET RBRACKET

%type <instruction> program stmts stmt loop
%type <token> syllable loop_mode
%type <sym> loop_start loop_end symbol

%destructor { free_instruct($$); } <instruction>

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
  | symbol   { $$ = mkinstruction($1); }
;

loop
  : loop_start stmts loop_end {
      $$ = mkinstruction($1);
      $$->next = $2;
      $$->other = mkinstruction($3);
      add_instruction($$->next, $$->other);
      $$->other->other = $$;
    }
;

loop_start
  : LBRACKET loop_mode { $$=symbol_from_syllable( $1, $2);}
  | loop_mode LBRACKET { $$=symbol_from_syllable( $1, $2);}

loop_end
  : RBRACKET loop_mode { $$=symbol_from_syllable( $1, $2);}
  | loop_mode RBRACKET { $$=symbol_from_syllable( $1, $2);}


symbol 
  : syllable syllable {
    $$=symbol_from_syllable( $1, $2);
  }
  | syllable {
    $$=symbol_from_syllable( NEUTRAL, $1);
  }

loop_mode 
  : PLUS
  | MINUS
  | NEUTRAL 
  | LEFT 
  | UP 
  | DOWN
  | RIGHT

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
