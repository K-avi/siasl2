#ifndef AST_H 
#define AST_H

typedef struct symbol{ 
    unsigned char syllable1 ; 
    unsigned char syllable2 ; 

}Symbol;

typedef struct instruction{

    int token;
    struct instruction* next; 
    struct instruction* other;

}instruction;

extern void free_instruct( instruction* parsed_prog);

extern  int parsed_to_int( instruction * parsed_prog);

extern instruction * mkinstruction(int token);
extern instruction * add_instruction( instruction* program, instruction * instruct);
 

extern void printprgm( instruction * prgm);

extern char token_to_char(int token);
/* change simply to doubly linked later down the line
typedef struct list{
    instruction *head; 
    instruction* tail; 
}list;
*/

enum { INT_NEUT=1 , INT_LEFT , INT_RIGHT, INT_UP , INT_DOWN , INT_PLUS, INT_MINUS, INT_MULT, 
INT_DIV ,INT_READ, INT_PRINT, INT_LBRACKET, INT_RBRACKET };

#endif