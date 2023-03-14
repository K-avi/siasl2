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

//doesnt handle overflows well (returns to 0 if u try to overfluw)
//will turn into a function for it to work properly ig?
#define OP_LEFT(index, matsize) ( ( (index) -1)% (matsize))
#define OP_RIGHT(index, matsize) ( ( (index) + 1) % (matsize))
#define OP_UP(index, matsize) ( ( (index) - DEFAULT_ROWSIZE ) % (matsize))
#define OP_DOWN(index, matsize) ( ( (index) + DEFAULT_ROWSIZE ) % (matsize))


#endif