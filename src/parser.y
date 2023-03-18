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
%type <token> syllable

%type <sym> loop_start loop_end symbol

%start program

%expect 49


%destructor { ; } <instruction>

%%

program
  : stmts { prog = $$; }
;

stmts
  : stmt        { $$ = $1; }
  | stmts stmt  { add_instruction($$ = $1, $2); }
  | stmts error { printf( "stmts error\n"); free_instruct($1) ; $$=NULL; YYABORT; }
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
  | loop_start loop_end{
      $$ = mkinstruction($1);
      $$->next = mkinstruction($2);
      $$->other = $$->next;
      
      $$->other->other = $$;
    }
  
;

loop_start
  : LBRACKET PLUS { $$=symbol_from_syllable( $1, $2);}
  | PLUS LBRACKET { $$=symbol_from_syllable( $1, $2);}
  
  | LBRACKET MINUS { $$=symbol_from_syllable( $1, $2);}
  | MINUS LBRACKET { $$=symbol_from_syllable( $1, $2);}

  | LBRACKET LEFT { $$=symbol_from_syllable( $1, $2);}
  | LEFT LBRACKET { $$=symbol_from_syllable( $1, $2);}

  | LBRACKET UP { $$=symbol_from_syllable( $1, $2);}
  | UP LBRACKET { $$=symbol_from_syllable( $1, $2);}

  | LBRACKET DOWN { $$=symbol_from_syllable( $1, $2);}
  | DOWN LBRACKET { $$=symbol_from_syllable( $1, $2);}

  | LBRACKET RIGHT { $$=symbol_from_syllable( $1, $2);}
  | RIGHT LBRACKET { $$=symbol_from_syllable( $1, $2);}

  | LBRACKET NEUTRAL { $$=symbol_from_syllable( $1, $2);}
  | NEUTRAL LBRACKET { $$=symbol_from_syllable( $1, $2);}



loop_end
  : RBRACKET PLUS { $$=symbol_from_syllable( $1, $2);}
  | PLUS RBRACKET { $$=symbol_from_syllable( $1, $2);}
  
  | RBRACKET MINUS { $$=symbol_from_syllable( $1, $2);}
  | MINUS RBRACKET { $$=symbol_from_syllable( $1, $2);}

  | RBRACKET LEFT { $$=symbol_from_syllable( $1, $2);}
  | LEFT RBRACKET { $$=symbol_from_syllable( $1, $2);}

  | RBRACKET UP { $$=symbol_from_syllable( $1, $2);}
  | UP RBRACKET { $$=symbol_from_syllable( $1, $2);}

  | RBRACKET DOWN { $$=symbol_from_syllable( $1, $2);}
  | DOWN RBRACKET { $$=symbol_from_syllable( $1, $2);}

  | RBRACKET RIGHT { $$=symbol_from_syllable( $1, $2);}
  | RIGHT RBRACKET { $$=symbol_from_syllable( $1, $2);}

  | RBRACKET NEUTRAL { $$=symbol_from_syllable( $1, $2);}
  | NEUTRAL RBRACKET { $$=symbol_from_syllable( $1, $2);}



symbol 
  : syllable syllable  {
    $$=symbol_from_syllable( $1, $2);
  }  
  | syllable {
    $$=symbol_from_syllable( NEUTRAL, $1);
  }


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
