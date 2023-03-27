#ifndef CELLS_H 
#define CELLS_H

#include <stdint.h>

typedef int32_t CELL;

typedef struct matrix{
    uint32_t size; //it's a square matrix bc I said so. So this is the ROW SIZE
    uint32_t curindex;
    CELL * mat;
}CELLMATRIX;


extern CELLMATRIX* init_mat(unsigned size);
extern void free_mat(CELLMATRIX * mat);

extern void print_mat( CELLMATRIX* mat);

#define DEFAULT_MATSIZE 32400 // 180^2
#define DEFAULT_ROWSIZE 16



#endif