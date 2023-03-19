#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "data.h"
#define INIT_SIZE 2;

struct array {
    footpath_t **F;
    int size;
    int n;
};
typedef struct array array_t;

array_t *createArray();

void ensureArraySize(array_t *arr);

void appendArray(array_t *arr, footpath_t *n);

void arraySortInsert(array_t *arr, footpath *n);

int getFootpathID(footpath_t *n);

void arrayFree(array_t *arr);

int uniqueSortedArray(array_t *array, footpath_t *footpath);
#endif
