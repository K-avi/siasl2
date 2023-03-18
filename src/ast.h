#ifndef AST_H 
#define AST_H

#include "parser.tab.h"

typedef struct symbol Symbol;

typedef struct instruction{

    Symbol symbol ;
    struct instruction* next; 
    struct instruction* other;

}instruction;


typedef struct Double_instruction{ 
    Symbol symbol ;
    struct Double_instruction* next; 
    struct Double_instruction * prev;
    struct Double_instruction* other;

}dinstruction;

typedef struct prog{
    dinstruction * head; 
    dinstruction * tail;
}program;


extern void free_instruct( instruction* parsed_prog);

extern  int parsed_to_int( instruction * parsed_prog);

extern instruction * mkinstruction(Symbol sym );
extern instruction * add_instruction( instruction* program, instruction * instruct);
 
 Symbol symbol_from_syllable( int tok1 , int  tok2);

extern void printprgm( instruction * prgm);

extern char token_to_char(int token);
/* change simply to doubly linked later down the line
typedef struct list{
    instruction *head; 
    instruction* tail; 
}list;
*/

enum { INT_NEUT=1 , INT_LEFT , INT_RIGHT, INT_UP , INT_DOWN , INT_PLUS, INT_MINUS, INT_MULT, 
INT_DIV ,INT_READ, INT_PRINT, INT_LBRACKET, INT_RBRACKET, INT_WILDCARD };

#endif