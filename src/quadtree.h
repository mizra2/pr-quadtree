#ifndef _QUADTREE_H_
#define _QUADTREE_H_
#include "array.h"
#include "list.h"

#define MAX_LINE_LENGTH 512
#define WHITE 0
#define BLACK 1
#define GREY 2
#define NUMQUADS 4

typedef struct {
    long double x;
    long double y;
} point2D;

typedef struct {
    point2D *bottomLeft;
    point2D *upperRight;
} rectangle2D;

typedef struct {
    point2D *point;
    array_t *footpaths;
} dataPoint;

typedef struct quadtreeNode quadtreeNode_t;

struct quadtreeNode {
    rectangle2D *rectangle;
    dataPoint *data;
    quadtreeNode_t **quadrants;
    int colour;
};

quadtreeNode_t *createQuadtree(rectangle2D *rectangle);

void quadtreeInsert(quadtreeNode_t *quadtree, dataPoint *data);

int inRectangle(quadtreeNode_t *quadtree, point2D *p);

int determineQuadrant(quadtreeNode_t *quadtree, point2D *p);

void *divideQuadTree(quadtreeNode_t *quadtree, int q);

rectangle2D *newRectangle(point2D *upper, point2D *lower);

point2D *newPoint(long double *x, long double *y);

int checkDuplicatePoint(quadtreeNode_t *quadtree, dataPoint *data);

point2D *getFootpathStart(footpath_t *n);

point2D *getFootpathEnd(footpath_t *n);

quadtreeNode_t *buildQuadtree(FILE *input, char *argv[], list_t *footpaths);

void searchQuadtree(quadtreeNode_t *quadtree, point2D *point, FILE *output);

void dictSearch(quadtreeNode_t *quadtree, FILE *output);

void processQuadrant(int q);

int rectangleOverlap(rectangle2D *r1, rectangle2D *r2);

void queryDictSearch(quadtreeNode_t *quadtree, FILE *output);

void searchQuery(quadtreeNode_t *quadtree, rectangle2D *rectangle, FILE *output, array_t *array);

int pointinRectangle(point2D *point, rectangle2D *r2);

void freeQuadtree(quadtreeNode_t *quadtree);

void freeRectangle(rectangle2D *r);

void dataPointFree(dataPoint *d);

dataPoint *createDataPoint(point2D *point);

#endif
