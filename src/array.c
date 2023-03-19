#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "array.h"

// Creates an empty array with inital size of 2 
// Credit / Reference to Workshop 3.7 / 3.8 
array_t *createArray() {
    array_t *arr = malloc(sizeof(*arr));
    assert(arr);
    int size = 2;
    arr->size = size;
    arr->F = malloc(size * sizeof(*(arr->F)));
    assert(arr->F);
    arr->n = 0;
    return arr;
}

// Ensures an array has enough 'space' so we can insert another element
// Credit / Reference to Workshop 3.7 / 3.8
void ensureArraySize(array_t *arr) {
    if(arr->n == arr->size) {
        arr->size *= 2;
        arr->F = realloc(arr->F, arr->size * sizeof(*(arr->F)));
        assert(arr->F);
    }
}

// Add a footpath *n to the end of an array
// Inspired by Workshop 3.7 / 3.8
void appendArray(array_t *arr, footpath_t *n) {
    ensureArraySize(arr);
    arr->F[arr->n] = n;
    (arr->n)++;
}

// Ensures the size of an array and performs an insertion sort based on footpath attribue
// Credit / Reference to Workshop 3.7 / 3.8
void arraySortInsert(array_t *arr, footpath_t *n) {
    int i;
    ensureArraySize(arr);
    for (i = arr->n - 1; i >=0 && getFootpathID(n) < getFootpathID(arr->F[i]); i--) {
        arr->F[i + 1] = arr->F[i];
    }
    arr->F[i + 1] = n;
    arr->n++;
}

// Return footpath ID
int getFootpathID(footpath_t *n) {
    return n->footpath_id;
}

// Only insertion sort add the unique footpaths into an array 
int uniqueSortedArray(array_t *array, footpath_t *footpath) {
    for (int i = 0; i < array->n; i++) {
        if (array->F[i]->footpath_id == footpath->footpath_id) {
            return 0;
        }
    }
    arraySortInsert(array, footpath);
    return 1;
}
// Free array
void arrayFree(array_t *arr) {
	for (int i = 0; i < arr->n; i++) {
		free(arr->F[i]);
	}
	free(arr->F);
	free(arr);
}
