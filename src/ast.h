#ifndef AST_H 
#define AST_H

#include "parser.tab.h"

typedef struct symbol Symbol;




typedef struct instruction{ 
    Symbol symbol ;
    struct instruction * next; 
    struct instruction * prev;
    struct instruction * other;

}instruction;

typedef struct program{
    instruction * head; 
    instruction * tail;
}program;


Symbol symbol_from_syllable( int tok1 , int  tok2);


extern char token_to_char(int token);


extern instruction * mkinstruction( Symbol sym);
extern program* initProg();

extern void insertHead( program * prog,  instruction * newH);

extern void insertTail( program * prog, instruction * newT);

extern void free_prog (program * prog);

extern void printprgm( program* prgm);

extern void progMerge (program * prog, instruction * list);

extern void mergeInstruction( instruction * list1, instruction * list2);

extern void free_instruct( instruction * list);

enum { INT_NEUT=1 , INT_LEFT , INT_RIGHT, INT_UP , INT_DOWN , INT_PLUS, INT_MINUS, INT_MULT, 
INT_DIV ,INT_READ, INT_PRINT, INT_LBRACKET, INT_RBRACKET, INT_WILDCARD };

#endif