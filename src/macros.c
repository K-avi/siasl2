#include "macros.h"
#include "ast.h"
#include <stdlib.h>

#include <stdio.h>

program * instructToProg( instruction * instruct){
    /*
    turns a list of instructions into a valid prog.
    */
    if (!instruct) return NULL;

    program* ret= (program*) malloc(sizeof(program));
    if(!ret) return NULL;

    ret->head=instruct;

    while(instruct->next){
        instruct=instruct->next;
    }
    ret->tail=instruct;
    return ret;
}//tested ; works 

macroentry * mkMacroEntry ( Symbol sym, program * prog){
    /*
    allocates a macroentry and sets it sym to sym and prog to prog 
    WARNING: doesn't duplicate prog argument so watch out for taht 

    also returns NULL if malloc fails.
    */
    macroentry * ret= (macroentry*) malloc(sizeof( macroentry));
    if(!ret) return NULL;

    ret->sym=sym; 
    ret->prog=prog;

    return ret;
}//not tested;


void free_macroentry( macroentry * entry){
    /*
    frees a macroentry AND the program it contains.
    */
    if(!entry) return;

    free_prog(entry->prog);
    free(entry);
}


entryarray * initArr( int size){
    /*
    initialises an entry array ; 
    throws NULL if malloc fails.
    */

    entryarray * ret= (entryarray* ) malloc(size* sizeof(entryarray));
    if(!ret) return NULL;

    ret->entries= (macroentry**) malloc(size* sizeof(macroentry*));
    if(!ret->entries){
        free(ret);
        return NULL;
    }
    ret->currentries=0;
    ret->maxentries=size;

    return ret;
}//not tested 

void free_entryarray ( entryarray * array){

    if(!array) return;

    if(array->entries){
        for(int i =0 ; i<array->currentries; i++){
            free_macroentry(array->entries[i]);
           
        }
    }
    free(array->entries);
    free(array);
}//not tested

void appArr( entryarray * arr, macroentry * element){
    /*
    adds element to arr ; 
    WARNING : doesnt copy element->prog 
    be carefull with that
    */
    if(! (arr && element)) return;
    
    if(arr->currentries==arr->maxentries){
      
        arr->entries= (macroentry**)realloc( arr->entries, 4 * sizeof(macroentry*));
        arr->maxentries+=4;
    }
   
    arr->entries[arr->currentries] = element;
    arr->currentries++;

}//not tested
//doesnt handle realloc (does it need to though????)


macrotable * init_table( unsigned short size, unsigned arrsize){
    /*
    initialises a table to store macro functions. 
    returns null if any malloc fails so watch out for that.
    */

    macrotable * ret= (macrotable* ) malloc(sizeof(macrotable));
    if(!ret) return NULL; 

    ret->table= (entryarray**) calloc(size , sizeof(entryarray*));
    if(!ret->table){
        free(ret);
        return NULL;
    }

    ret->size=size;
    ret->arrsize=arrsize;

    return ret;
}//tested ; works

void free_table( macrotable * mtable){
    /*
    frees a macrotable.
    */
    if(!mtable) return ;
    for(unsigned i=0; i <mtable->size; i++ ){
        
        free_entryarray(mtable->table[i]);
    }
    free(mtable->table);
    free(mtable);
}//not tested


void appTable ( macrotable * mtable, macroentry * entry){
    /*
    adds an entry to a mtable
    */
    if(! (mtable && entry)) return;

    unsigned key= hashSymbol(entry->sym, mtable->size);

  //  printf("key is %d , %p \n", key, mtable->table[key]);

    if(! (mtable->table[key])) {
        mtable->table[key]=initArr(_ARRENT_DEF_SIZE);
    }
    // printf("key is %d , %p \n", key, mtable->table[key]);
    appArr(mtable->table[key], entry);

   // free_macroentry(entry);
    
}//not tested 
//doesnt handle case where it's already defined! !!!!!!!!!!!!!





unsigned hashSymbol( Symbol sym, unsigned size){

    if( !size) return 0;
    unsigned hash= sym + (sym>>4) * 3;
    return hash%size;
}//not tested 

program * findProg( macrotable * mtable, Symbol sym){
    /*
    returns the reference of a program associated w a symbol in mtable or 
    null if not present or 
    table is NULL.
    */
    if(!mtable) return NULL;

    unsigned key = hashSymbol(sym, mtable->size);
    if(mtable->table[key]){
        for (int i=0; i<mtable->table[key]->currentries; i++){

            if(mtable->table[key]->entries[i]->sym==sym){

                return mtable->table[key]->entries[i]->prog;
            }
        }
    }

    return NULL;
}//not tested 