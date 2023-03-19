#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "quadtree.h"


 /*  Special Thanks / Credit to "Anh Vo" for assistance and
 *      details on a quadtree implementation
 *   Including insertion, searching and query searchs. */

// Create a new empty quadtree
quadtreeNode_t *createQuadtree(rectangle2D *rectangle) {
    quadtreeNode_t *newTree = malloc(sizeof(*newTree));
    newTree->rectangle = rectangle;
    newTree->quadrants = (quadtreeNode_t **)malloc(sizeof(quadtreeNode_t*) * 4);
    for (int i = 0; i < 4; i++) {
        newTree->quadrants[i] = NULL;
    }
    newTree->data = NULL;
    newTree->colour = WHITE;
    return newTree;
}

// Insert a point in a quadtree
void quadtreeInsert(quadtreeNode_t *quadtree, dataPoint *data) {
    if (!inRectangle(quadtree, data->point)) {
        return;
    }
    // If point not in the rectangle, return
    if (quadtree->colour == WHITE) {
        quadtree->data = data;
        quadtree->colour = BLACK;
    }
    // If it's black we know we are at a leaf node
    else if (quadtree->colour == BLACK) {
        // If duplicate, append it to the already existing array
        if(checkDuplicatePoint(quadtree, data)) {
            for (int i = 0;i < data->footpaths->n;i++) {
                arraySortInsert(quadtree->data->footpaths, data->footpaths->F[i]);
            }
            // Free the data
            free(data->footpaths->F);
            free(data->footpaths);
            free(data->point);
            free(data);
        } else {
            // Else, we are determineQuadrant of the old point 
            int i = determineQuadrant(quadtree, quadtree->data->point);
            // if it's null, we divide the tree
            if (quadtree->quadrants[i] == NULL) {
                divideQuadTree(quadtree, i);
            }
            // Insert the old point
            // Repeat with the new point
            quadtreeInsert(quadtree->quadrants[i], quadtree->data);

            int j = determineQuadrant(quadtree, data->point);
            if(quadtree->quadrants[j] == NULL) {
                divideQuadTree(quadtree, j);
            }
            quadtreeInsert(quadtree->quadrants[j], data);
            // Set the point to grey
            quadtree->colour = GREY;
        }
    }
    else if (quadtree->colour==GREY) {
        // If it's grey, recursively divide
        int k = determineQuadrant(quadtree, data->point);
        if(quadtree->quadrants[k] == NULL) {
            divideQuadTree(quadtree, k);
        }
        quadtreeInsert(quadtree->quadrants[k], data);
    }
}
// Function to check if the point is in the rectangle
int inRectangle(quadtreeNode_t *quadtree, point2D *p) {
    return (p->x > quadtree->rectangle->bottomLeft->x && p->x <= quadtree->rectangle->upperRight->x &&
    p->y >= quadtree->rectangle->bottomLeft->y && p->y < quadtree->rectangle->upperRight->y);
}
// Determine what quadrant the point lies in
int determineQuadrant(quadtreeNode_t *quadtree, point2D *p) {
    if (p->x <= ((quadtree->rectangle->upperRight->x + quadtree->rectangle->bottomLeft->x) / 2)) {
        if (p->y < ((quadtree->rectangle->upperRight->y + quadtree->rectangle->bottomLeft->y) / 2)) {
            return 0;
        }
        else return 1;
    } else {
        if(p->y >= ((quadtree->rectangle->upperRight->y + quadtree->rectangle->bottomLeft->y) / 2)) {
            return 2;
        } else {
            return 3;
        }
    }
}
// Creates a new rectangle quadtree node
void *divideQuadTree(quadtreeNode_t *quadtree, int q) {
    long double halfX = (quadtree->rectangle->bottomLeft->x + quadtree->rectangle->upperRight->x) / 2.0;
    long double halfY = (quadtree->rectangle->bottomLeft->y + quadtree->rectangle->upperRight->y) / 2.0;
    if (q == 0) {
        point2D *upper = newPoint(&halfX, &halfY);
        point2D *lower = newPoint(&quadtree->rectangle->bottomLeft->x, &quadtree->rectangle->bottomLeft->y);
        rectangle2D *new = newRectangle(upper, lower);
        quadtree->quadrants[0] = createQuadtree(new);
    } else if(q == 1) {
        point2D *upper = newPoint(&halfX, &(quadtree->rectangle->upperRight->y));
        point2D *lower = newPoint(&(quadtree->rectangle->bottomLeft->x), &halfY);
        rectangle2D *tmp = newRectangle(upper, lower);
        quadtree->quadrants[1] = createQuadtree(tmp);
    } else if(q == 2) {
        point2D *upper = newPoint(&quadtree->rectangle->upperRight->x, &quadtree->rectangle->upperRight->y);
        point2D *lower = newPoint(&halfX, &halfY);
        rectangle2D *tmp = newRectangle(upper, lower);
        quadtree->quadrants[2] = createQuadtree(tmp);
    } else {
        point2D *upper = newPoint(&(quadtree->rectangle->upperRight->x), &halfY);
        point2D *lower = newPoint(&halfX, &(quadtree->rectangle->bottomLeft->y));
        rectangle2D *tmp = newRectangle(upper, lower);
        quadtree->quadrants[3] = createQuadtree(tmp);
    }
    return quadtree;
}

// Returns a new rectangle based on 2 points
rectangle2D *newRectangle(point2D *upper, point2D *lower) {
    rectangle2D *temp = malloc(sizeof(*temp));
    temp->upperRight = upper;
    temp->bottomLeft = lower;
    return temp;
}

// Returns new point based on x and y.
point2D *newPoint(long double *x, long double *y) {
    point2D *temp = malloc(sizeof(*temp));
    temp->x = *x;
    temp->y = *y;
    return temp;
}

// checks if the point already exists in the quadtree
int checkDuplicatePoint(quadtreeNode_t *quadtree, dataPoint *data) {
    if (quadtree->data->point == NULL) return 0;
    for (int i = 0; i < quadtree->data->footpaths->n;i++) {
        long double x = quadtree->data->point->x;
        long double y = quadtree->data->point->y;
        if((x == data->point->x && y == data->point->y)) {
            return 1;
        }
    }
    return 0;
}

// Gets the start of the footpath as a point
point2D *getFootpathStart(footpath_t *n) {
    long double x = n->start_lon;
    long double y = n->start_lat;
    point2D *temp = newPoint(&x, &y);
    return temp;
}

// Gets the end of the footpath as a point
point2D *getFootpathEnd(footpath_t *n) {
    long double x = n->end_lon;
    long double y = n->end_lat;
    point2D *temp = newPoint(&x, &y);
    return temp;
}

// Loops through the records in the linked list and builds a quad tree
quadtreeNode_t *buildQuadtree(FILE *input, char *argv[], list_t *footpaths) {
    char *ptr;
    long double x1 = strtold(argv[4], &ptr);
    long double y1 = strtold(argv[5], &ptr);
    long double x2 = strtold(argv[6], &ptr);
    long double y2 = strtold(argv[7], &ptr);

    point2D *bottomLeft = newPoint(&x1, &y1);
    point2D *upperRight = newPoint(&x2, &y2);

    rectangle2D *test = newRectangle(upperRight, bottomLeft);
    quadtreeNode_t *newTree = createQuadtree(test);

    node_t *current = footpaths->head;
    while(current) {
        // Get footpaths start and end coords
        long double startX = current->data->start_lon;
        long double startY = current->data->start_lat;

        long double endX = current->data->end_lon;
        long double endY = current->data->end_lat;

        point2D *p1 = newPoint(&startX, &startY);
        dataPoint *point1 = createDataPoint(p1);

        point2D *p2 = newPoint(&endX, &endY);
        dataPoint *point2 = createDataPoint(p2);

        arraySortInsert(point1->footpaths, current->data);
        arraySortInsert(point2->footpaths, current->data);

        quadtreeInsert(newTree, point1);
        quadtreeInsert(newTree, point2);
        current = current->next;
    }
    return newTree;
}

// Search the quadtree based on a point querty
void searchQuadtree(quadtreeNode_t *quadtree, point2D *point, FILE *output) {
    if (!inRectangle(quadtree, point)) {
        return;
    }
    int i = determineQuadrant(quadtree, point);
    // If it's black, we found the point
    if (quadtree->colour == BLACK) {

        long double x1 = point->x;
        long double y1 = point->y;
        
        long double x2 = quadtree->data->point->x;
        long double y2 = quadtree->data->point->y;
        // Compare using EPSILON value of 10e-10 
        if (fabs(x1 - x2) < 10e-10 && fabs(y1 - y2) < 10e-10) {
            for(int i = 0; i < quadtree->data->footpaths->n;i++) {
                printFootpathOutput(quadtree->data->footpaths->F[i], output);
            }
        }
    } 
    // If it's grey we at root, and need to recursively check
    if (quadtree->colour == GREY) {
        processQuadrant(i);
        searchQuadtree(quadtree->quadrants[i], point, output);
    }
}

// Read from sdtin input and search for that input
void dictSearch(quadtreeNode_t *quadtree, FILE *output) {
    char *line = NULL;
    size_t lineBufferLength = 0;
    int c;
    while ((c = getline(&line, &lineBufferLength, stdin)) > 0) {
        if (line[c-1] == '\n') line[c - 1] = '\0';
        fprintf(output, "%s\n", line);
        printf("%s --> ", line);
        long double x; 
        long double y;
        sscanf(line, "%Lf %Lf\n", &x, &y);
        point2D *tmp = newPoint(&x, &y);
        searchQuadtree(quadtree, tmp, output);
        free(tmp);
        printf("\n");
    }
    free(line);
}
// Print the quadrant
void processQuadrant(int q) {
    switch (q) {
        case 0:
            printf("SW ");
            break;
        case 1:
            printf("NW ");
            break;
        case 2:
            printf("NE ");
            break;
        case 3:
            printf("SE ");
            break;
    }
}

// Checks if 2 rectangles overlap by checking their bottm and upper right points
int rectangleOverlap(rectangle2D *r1, rectangle2D *r2) {
    if (r1 == NULL || r2 == NULL) {
        return 0;
    }

    if (r1->bottomLeft->x == r1->upperRight->x || r1->bottomLeft->y == r1->upperRight->y ||
    r2->bottomLeft->x == r2->upperRight->x || r2->bottomLeft->y == r2->upperRight->y) {        
        return 0;
    }

    return !((r1->upperRight->x < r2->bottomLeft->x) || (r1->upperRight->y < r2->bottomLeft->y) || 
    (r1->bottomLeft->x > r2->upperRight->x) || (r1->bottomLeft->y > r2->upperRight->y));
}

// Searchs a quadtree based on a query
void queryDictSearch(quadtreeNode_t *quadtree, FILE *output) {
    char *line = NULL;
    size_t lineBufferLength = 0;
    int c;
    // Reads a file and returns the results based on that query
    while ((c = getline(&line, &lineBufferLength, stdin)) > 0) {
        array_t *array = createArray();
        if (line[c-1] == '\n') line[c - 1] = '\0';
        fprintf(output, "%s\n", line);
        printf("%s --> ", line);

        long double x1; 
        long double y1;
        long double x2;
        long double y2;

        sscanf(line, "%Lf %Lf %Lf %Lf\n", &x1, &y1, &x2, &y2);
        point2D *bottomLeft = newPoint(&x1, &y1);
        point2D *upperRight = newPoint(&x2, &y2);
        rectangle2D *tmp = newRectangle(upperRight, bottomLeft);
        searchQuery(quadtree, tmp, output, array);
        // Process the data
        for (int i = 0; i < array->n;i++) {
            printFootpathOutput(array->F[i], output);
        }

        printf("\n");

        free(tmp->bottomLeft);
        free(tmp->upperRight);
        free(tmp);
        free(array->F);
        free(array);
    }
    free(line);
}

// Fuction that finds all points in a region / rectangle
void searchQuery(quadtreeNode_t *quadtree, rectangle2D *rectangle, FILE *output, array_t *array) {
    // If it's empty, return
    if (quadtree->colour == WHITE) return;
    // If they don't overlap, return 
    if (!rectangleOverlap(rectangle, quadtree->rectangle)) return;
    // If it's black, we found a leaf node and search for the point inside it
    if (quadtree->colour == BLACK) {
        if(pointinRectangle(quadtree->data->point, rectangle)) {
            for (int i = 0; i < quadtree->data->footpaths->n;i++)
                uniqueSortedArray(array, quadtree->data->footpaths->F[i]);
            }
    }
    // If it's grey, we recursively search through all the quadrants 
    if (quadtree->colour == GREY) {
        if (quadtree->quadrants[0] != NULL) {
            if (quadtree->quadrants[0]->colour != WHITE) {
                if (rectangleOverlap(rectangle, quadtree->quadrants[0]->rectangle)) processQuadrant(0);
            }
            searchQuery(quadtree->quadrants[0], rectangle, output, array);
        }
        if (quadtree->quadrants[1] != NULL) {
            if (quadtree->quadrants[1]->colour != WHITE) {
                if (rectangleOverlap(rectangle, quadtree->quadrants[1]->rectangle)) processQuadrant(1);
            }
            searchQuery(quadtree->quadrants[1], rectangle, output, array);
        }
        if (quadtree->quadrants[2] != NULL) {
            if (quadtree->quadrants[2]->colour != WHITE) {
                if (rectangleOverlap(rectangle, quadtree->quadrants[2]->rectangle)) processQuadrant(2);
            }
            searchQuery(quadtree->quadrants[2], rectangle, output, array);
        }
        if (quadtree->quadrants[3] != NULL) {
            if (quadtree->quadrants[3]->colour != WHITE) {
                if (rectangleOverlap(rectangle, quadtree->quadrants[3]->rectangle)) processQuadrant(3);
            }
            searchQuery(quadtree->quadrants[3], rectangle, output, array);
        }
    }
}

// Check if the point actually lies specifically in that rectangle
int pointinRectangle(point2D *point, rectangle2D *r2) {
    if (point->x > r2->bottomLeft->x && point->y >= r2->bottomLeft->y && point->x <= r2->upperRight->x && point->y < r2->upperRight->y) return 1;
    return 0;
}

// Frees a quadtree
void freeQuadtree(quadtreeNode_t *quadtree){
    if (!quadtree) {
        return;
    }
    freeQuadtree(quadtree->quadrants[0]);
    freeQuadtree(quadtree->quadrants[1]);
    freeQuadtree(quadtree->quadrants[2]);
    freeQuadtree(quadtree->quadrants[3]);
    freeRectangle(quadtree->rectangle);
    free(quadtree->quadrants);
    if (quadtree->colour == BLACK) dataPointFree(quadtree->data);
    free(quadtree);
}

// Free a rectangle
void freeRectangle(rectangle2D *r) {
    free(r->upperRight);
    free(r->bottomLeft);
    free(r);
}
// free the data point
void dataPointFree(dataPoint *d) {
    free(d->point);
    free(d->footpaths->F);
    free(d->footpaths);
    free(d);
}
// Create a new data point
dataPoint *createDataPoint(point2D *point) {
    dataPoint *p = malloc(sizeof(*p));
    p->footpaths = createArray();
    p->point = point;
    return p;
}
