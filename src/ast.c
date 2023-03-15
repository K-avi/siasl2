#include "ast.h"
#include <stdio.h>
#include <stdlib.h>


int bison_token_to_internal( int token){
    /*
    converts flex/bison generated token to internal rep; 
    will be usefull down the line cuz multiple parsers n shit
    */
   
    switch (token){
        case PRINT: return INT_PRINT; break;
        case READ: return INT_READ; break;
        case LEFT: return INT_LEFT; break;
        case RIGHT: return INT_RIGHT; break; 
        case UP: return INT_UP; break;
        case DOWN: return INT_DOWN; break;
        case PLUS: return INT_PLUS; break;
        case MINUS: return INT_MINUS; break;
        case MULT: return INT_MULT; break;
        case DIV: return INT_DIV; break;
        case NEUTRAL: return INT_NEUT;
        case LBRACKET: return INT_LBRACKET; break;
        case RBRACKET: return INT_RBRACKET; break;
        case WILDCARD: return INT_WILDCARD ; break;
        default: return INT_NEUT; 
        
    }

    return INT_NEUT;
}//not tested

Symbol symbol_from_syllable( int tok1 , int  tok2){
    
  

     unsigned short i_tok1= bison_token_to_internal(tok1), i_tok2=bison_token_to_internal(tok2);
     //printf("in sym from syll %d %d %u %u\n", tok1, tok2, i_tok1 , i_tok2);
     if (i_tok1 > i_tok2) {
        Symbol ret= {i_tok2, i_tok1};
        return ret;
     }else {
        Symbol ret= { i_tok1, i_tok2};
        return ret;
     }

}

instruction * mkinstruction( Symbol sym) {
   // printf("in mkinstr %d %d\n", sym.token1, sym.token2);
  instruction* ret = (instruction*)malloc(sizeof(instruction));
  ret->symbol.token1 = sym.token1;
  ret->symbol.token2 = sym.token2;
  ret->next = NULL;
 
  ret->other = NULL;
  return ret;
}

//append would be better if list was doubly linked cuz O(1) vs O(n) and shit...
instruction * add_instruction( instruction* program, instruction * instruct){
   
    instruction* curr = program;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = instruct;
    return program;
}


void free_instruct( instruction* parsed_prog){
    
    if(!parsed_prog) return;
    
    instruction* tmp= parsed_prog, *tmp1=parsed_prog;
  
    while (tmp){

        tmp1=tmp1->next;

        free(tmp);

        tmp=tmp1;
    }
}



int parsed_to_int( instruction * parsed_prog){
    /*
    turns the list of parsed instruction tokens retrieved via flex/bison 
    into the list of internal instructions. 
    This is gonna be VERY usefull down the line cuz there will be multiple parsers for the language
    */
    instruction* curr= parsed_prog;
   
    while(curr){
       
        int curtoken1= bison_token_to_internal(curr->symbol.token1), curtoken2= bison_token_to_internal(curr->symbol.token2) ;
        if(curtoken1<=0 || curtoken2<=0 ){

            return curtoken1 | curtoken2; //return token error
        }

        curr->symbol.token1=curtoken1;
        curr->symbol.token2=curtoken2;

        curr=curr->next; 
    }
    return 1; //returns OK
}//not tested


char token_to_char(int token){
    
    switch (token) {
    case INT_LEFT : return '<'; break;
    case INT_RIGHT: return '>'; break;
    case INT_UP : return '^'; break;
    case INT_DOWN : return 'v'; break;
    case INT_PLUS : return '+'; break;
    case INT_MINUS : return '-'; break;
    case INT_MULT : return '*'; break;
    case INT_DIV : return '/'; break;
    case INT_LBRACKET : return '['; break;
    case INT_RBRACKET : return ']'; break;
    case INT_NEUT: return '#'; break;
    case INT_READ: return ','; break;
    case INT_PRINT: return '.'; break;
    case INT_WILDCARD: return '?'; break;
    default : return 'x'; break;
    }

    return '?';
}

void printprgm( instruction * prgm){

    instruction * cur=prgm; 

    while(cur){
        printf("%c%c", token_to_char(cur->symbol.token1), token_to_char(cur->symbol.token2) );
        cur=cur->next;
    }
    printf("\n");
}


