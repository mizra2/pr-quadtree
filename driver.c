/*      Quadtree Implementation
 *    Written by: Milad Izra
 * 
 *  Special Thanks / Credit to "Anh Vo" for assistance 
 *       
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "quadtree.h"
#include "array.h"
#include "list.h"
#define MAX_LINE_LENGTH 512
#define DICT_SEARCH 1
#define QSEARCH 2

int processDataQuery(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[2], "r"); 
    FILE *output = fopen(argv[3], "w");
    int dictType = processDataQuery(argc, argv);
    list_t *footpaths = make_empty_list();
    buildFootpathList(input, footpaths);
    quadtreeNode_t *test = buildQuadtree(input, argv, footpaths);
    // runs respective query system
    switch (dictType) {
        case 3:
            dictSearch(test, output);
            freeQuadtree(test);
            freeFootpath(footpaths);
            break;
        case 4:
            queryDictSearch(test, output);
            freeQuadtree(test);
            freeFootpath(footpaths);
            break;
    }
    fclose(input);
    fclose(output);
    return 0;
}

int processDataQuery(int argc, char *argv[]) {
    return atoi(argv[1]);
}
