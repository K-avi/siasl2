#ifndef AST_H 
#define AST_H

#include "parser.tab.h"


typedef unsigned short Symbol;
struct program ;

typedef struct macroentry{
    Symbol sym; 
    struct program * prog;
}macroentry;

typedef struct entryarray{ 
    macroentry ** entries ;

    unsigned short currentries; 
    unsigned short maxentries;

}entryarray;

typedef struct macrotable{
    
    entryarray ** table;

    unsigned short size;
    unsigned short arrsize;

}macrotable;


typedef struct instruction{ 
    Symbol symbol ;
    struct instruction * next; 
    struct instruction * prev;
    struct instruction * other;

}instruction;

typedef struct program{
    instruction * head; 
    instruction * tail;

  //  macrotable * funs;
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

enum { INT_LPAR=0 , INT_NEUT , INT_LEFT , INT_RIGHT, INT_UP , INT_DOWN , INT_PLUS, INT_MINUS, INT_MULT, 
INT_DIV ,INT_READ, INT_PRINT, INT_LBRACKET, INT_RBRACKET, INT_WILDCARD , INT_RPAR};

#endif