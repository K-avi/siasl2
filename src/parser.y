/* based on brainflex grammar.y file https://github.com/lagerfeuer/brainflex/blob/main/src/grammar.y */
%{
  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include <ctype.h>
  #include <unistd.h>

  #include "ast.h"
  #include "macros.h"
  #include "globals.h"

  extern FILE* yyin;
  extern int yylex(void);
  extern int yyparse(void);
  void yyerror(const char *s);

  struct program * prog;

  extern struct macrotable * table;
  
%}


%union {
  struct instruction* instruction;
  int token;
  
  unsigned short sym;

  struct program * prog;
}

%token <token> PRINT READ LEFT RIGHT UP DOWN PLUS MINUS MULT DIV NEUTRAL WILDCARD LPAR RPAR INVALID_CHAR
%token <token> LBRACKET RBRACKET

%type <instruction> stmts stmt loop defun
%type <token> syllable 
%type <prog> program

%type <sym> loop_start loop_end symbol funstart funend

%start program



%destructor { ; } <instruction>

%%



program
  : stmts { prog=initProg();  progMerge(prog, $1); }
  | %empty { ; }
;




stmts
  : stmt    { $$ = $1; }
  | defun       { $$=$1 ; }
  | stmts stmt  { mergeInstruction($$ = $1, $2); }
  | stmts error { free_instruct($1) ; $$=NULL; YYABORT; }
  | stmts defun { mergeInstruction($$ = $1, $2);  }


;

defun 
  : funstart symbol stmts funend {

    $$=mkinstruction($1);
    $$->next=mkinstruction($2);
    $$->next->prev=$$;

    $$->next->other=$3;
    
    $$->next->next=mkinstruction($4);

    
    $$->other=$$->next->next;

    $$->other->prev=$$->next;

    $$->other->other=$$;
    }
 
;

funstart
  : NEUTRAL LPAR { $$=symbol_from_syllable( $1, $2);}
  | LPAR NEUTRAL {$$=symbol_from_syllable( $1, $2);}
;
funend 
  : NEUTRAL RPAR {$$=symbol_from_syllable( $1, $2);}
  | RPAR NEUTRAL {$$=symbol_from_syllable( $1, $2);}
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
      mergeInstruction($$->next, $$->other);
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
;


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
;


symbol 
  : syllable syllable  {
    $$=symbol_from_syllable( $1, $2);
  }

  | RBRACKET PRINT {$$=symbol_from_syllable($1 , $2);}
  | PRINT RBRACKET {$$=symbol_from_syllable($1 , $2);}
  | LBRACKET PRINT {$$=symbol_from_syllable($1 , $2);}
  | PRINT LBRACKET {$$=symbol_from_syllable($1 , $2);}

  | RBRACKET READ {$$=symbol_from_syllable($1 , $2);}
  | READ RBRACKET {$$=symbol_from_syllable($1 , $2);}
  | LBRACKET READ {$$=symbol_from_syllable($1 , $2);}
  | READ LBRACKET {$$=symbol_from_syllable($1 , $2);}

  | RBRACKET MULT {$$=symbol_from_syllable($1 , $2);}
  | MULT RBRACKET {$$=symbol_from_syllable($1 , $2);}
  | LBRACKET MULT {$$=symbol_from_syllable($1 , $2);}
  | MULT LBRACKET {$$=symbol_from_syllable($1 , $2);}

  | RBRACKET DIV {$$=symbol_from_syllable($1 , $2);}
  | DIV RBRACKET {$$=symbol_from_syllable($1 , $2);}
  | LBRACKET DIV {$$=symbol_from_syllable($1 , $2);}
  | DIV LBRACKET {$$=symbol_from_syllable($1 , $2);}

  | RBRACKET WILDCARD {$$=symbol_from_syllable($1 , $2);}
  | WILDCARD RBRACKET {$$=symbol_from_syllable($1 , $2);}
  | LBRACKET WILDCARD {$$=symbol_from_syllable($1 , $2);}
  | WILDCARD LBRACKET {$$=symbol_from_syllable($1 , $2);}

  | LPAR syllable {$$=symbol_from_syllable( $1, $2);}
  | syllable LPAR {{$$=symbol_from_syllable( $2, $1);}}
  | RPAR syllable {$$=symbol_from_syllable( $2, $1);}
  | syllable RPAR {$$=symbol_from_syllable( $2, $1);}

  | NEUTRAL syllable {$$=symbol_from_syllable( $2, $1);};
  | syllable NEUTRAL {$$=symbol_from_syllable( $2, $1);}
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
  | WILDCARD
;

%%


void yyerror(const char* s) {

  fprintf(stderr, "error: %s", s);
}
