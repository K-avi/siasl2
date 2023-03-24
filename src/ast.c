#include "ast.h"
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>


void free_instruct ( instruction * instruct){
    if(!instruct) return;

    instruction * tmp1= instruct, * tmp2=instruct;
    while(tmp1){
        tmp2=tmp2->next; 
        free(tmp1);

        tmp1=tmp2;
    }
}//not tested 


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
        case LPAR : return INT_LPAR; break;
        case RPAR: return INT_RPAR ; break;

        default: return INT_NEUT; 
        
    }

    return INT_NEUT;
}//not tested

Symbol symbol_from_syllable( int tok1 , int  tok2){
    
  

     unsigned short i_tok1= bison_token_to_internal(tok1), i_tok2=bison_token_to_internal(tok2);
  //   printf("in sym from syll %d %d %u %u\n", tok1, tok2, i_tok1 , i_tok2);
     if (i_tok1 > i_tok2) {
        Symbol ret= i_tok2 | ( i_tok1<<4);
        return ret;
     }else {
        Symbol ret=  i_tok1|( i_tok2<<4);
        return ret;
     }

}


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
    case INT_LPAR : return '('; break;
    case INT_RPAR : return ')'; break;
    default : return 'x'; break;
    }

    return '?';
}

instruction * mkinstruction( Symbol sym){
    /*
    */
    instruction * ret = (instruction*) malloc(sizeof(instruction));

    ret->symbol=sym;
    ret->next=NULL;
    ret->prev=NULL; 

    ret->other=NULL;

    return ret;
}

program* initProg(){
    /*
    called once at start of exec
    */
    program * ret= (program*) malloc(sizeof(program));

    ret->head=NULL; 
    ret->tail=NULL;

    return ret;
}

void insertHead( program * prog,  instruction * newH){
    /*
    not used atm; might be relevant later?
    */
    if(! (newH && prog) ) return;
    newH->next= prog->head;
    newH->prev=NULL;

    if((prog->head)){
       prog->head->prev=newH;
    }else{
        
    }
    prog->head=newH;
}//not tested 

void insertTail( program * prog, instruction * newT){
    /*
    not used atm ; might be relevant later?
    */
    if(! (newT && prog) ) return;
    newT->next=NULL;

    if(!(prog->head)){
        newT->prev=NULL;
        prog->head=newT;
    }else{
        prog->tail->next=newT;
    }
    prog->tail=newT;

}//not tested

void free_prog (program * prog){
    /*
    if error handler is set, will free the potential instruction * from a defun type statement.
    */
    if(!prog) return;

    instruction * tmp1= prog->head, * tmp2=prog->head;

    while(tmp1){
        tmp2=tmp2->next; 

        free(tmp1);

        tmp1=tmp2;
    }

    free(prog);
}//tested; works


void printprgm( program* prgm){
    /*
    not used in prog but usefull when debugging
    */
    if(!prgm){

        printf("empty prog\n");
        return ;
    }
    instruction * cur=prgm->head; 

    while(cur){
        printf("%c%c", token_to_char(cur->symbol), token_to_char( (cur->symbol>>4) & 0xF));
        cur=cur->next;
    }
    printf("\n");
}//not tested


void mergeInstruction( instruction * list1, instruction * list2){
    /*
    adds list2 to the end of list1 
    used in parser
    */
    if (! (list1 && list2)) return;

    instruction * tmp=list1; 

    while(tmp->next){
        tmp=tmp->next;
    }   

    tmp->next=list2; 
    list2->prev= tmp;
}//tested; works


void progMerge (program * prog, instruction * list){
    /*
    used in parser ; works ; appends list to the tail of prog and sets it's new tail 
    to the end of list
    different cases : prog is empty or 
    prog has one or more elements 
    */
    if(! (prog && list)) return;

    
    if (! (prog->tail && prog->head)){ //case : progempty
        prog->head= list;

        instruction * tmp= list; 

        while(tmp->next){
            tmp=tmp->next; 
        }
        prog->tail= tmp;
    }else{ //at least one element in prog; tail has to be changed to become last element of list 

        instruction * oldTail= prog->tail;

        oldTail->next= list; 
        list->prev= oldTail;

        instruction * tmp= list; 

        while(tmp->next){
            tmp=tmp->next;
        }

        prog->tail=tmp;
    }
    
}//tested; seems ok