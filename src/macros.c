#include "macros.h"
#include "ast.h"
#include <stdlib.h>

macrotable * init_table( unsigned short size, unsigned arrsize){
    /*
    initialises a table to store macro functions. 
    returns null if any malloc fails so watch out for that.
    */

    macrotable * ret= (macrotable* ) malloc(sizeof(macrotable));
    if(!ret) return NULL; 

    ret->table= (entryarray*) calloc(size , sizeof(entryarray));
    if(!ret->table){
        free(ret);
        return NULL;
    }

    ret->size=size;
    ret->arrsize=arrsize;

    return ret;
}//not tested

void free_entryarray ( entryarray * array){

    if(!array) return;

    if(array->entries)free(array->entries);
    free(array);
}

void free_table( macrotable * mtable){
    /*
    frees a macrotable.
    */
    if(!mtable) return ;
    free_entryarray(mtable->table);
    free(mtable);
}//not tested


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

entryarray * initArr( int size){
    /*
    initialises an entry array ; 
    throws NULL if malloc fails.
    */

    entryarray * ret= (entryarray* ) malloc(size* sizeof(entryarray));
    if(!ret) return NULL;

    ret->entries= calloc(size, sizeof(macroentry));
    if(!ret->entries){
        free(ret);
        return NULL;
    }
    ret->currentries=0;
    ret->maxentries=size;

    return ret;
}//not tested 

void appArr( entryarray * arr, macroentry * element){
    /*
    adds element to arr ; 
    WARNING : doesnt copy element->prog 
    be carefull with that
    */
    if(! (arr && element)) return;
    arr->entries[arr->currentries].sym=element->sym;
    arr->entries[arr->currentries].prog=element->prog;

    arr->currentries++;
}//not tested
//doesnt handle realloc (does it need to though????)


unsigned hashSymbol( Symbol sym, unsigned size){
    if( !size) return 0;

    return sym%size;
}//not tested 

void appTable ( macrotable * mtable, macroentry * entry){
    /*
    adds an entry to a mtable
    */
    if(! (mtable && entry)) return;

    unsigned key= hashSymbol(entry->sym, mtable->size);

    appArr(&mtable->table[key], entry);
    
}//not tested 
//doesnt handle case where it's already defined! !!!!!!!!!!!!!

program * findProg( macrotable * mtable, Symbol sym){
    /*
    returns the reference of a program associated w a symbol in mtable or 
    null if not present or 
    table is NULL.
    */
    if(!mtable) return NULL;

    unsigned key = hashSymbol(sym, mtable->size);

    for (int i=0; i<mtable->table[key].currentries; i++){
        if(mtable->table[key].entries[i].sym==sym){
            return mtable->table[key].entries[i].prog;
        }
    }

    return NULL;
}//not tested 