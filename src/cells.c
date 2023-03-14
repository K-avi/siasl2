#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "cells.h"


CELLMATRIX* init_mat(unsigned row_size){

    if(!row_size) return NULL;

    CELLMATRIX * ret= (CELLMATRIX*) malloc(sizeof(CELLMATRIX));

    if(!ret) return NULL; //checks for allocation problem

    unsigned mat_size= row_size*row_size;

    ret->mat= (CELL*) calloc(mat_size, sizeof( CELL));

    ret->size= row_size;
    ret->curindex= (mat_size)/2;

    return ret;
}


void free_mat(CELLMATRIX * mat){

    if(!mat) return;
    if(mat->mat) free(mat->mat);

    free(mat);
}

void print_mat( CELLMATRIX* mat){
    
    for(uint32_t i=0; i<mat->size*mat->size; i++){
        printf("%d ", mat->mat[i]);
        if(!(i%mat->size)) printf("\n");
        
    }
}