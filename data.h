#ifndef _DATA_H_
#define _DATA_H_
#include <stdio.h>

#define MAX_LINE_LENGTH 512

typedef struct Footpath 
{
    int footpath_id;
    char *address;
    char *clue_sa;
    char *asset_type;
    double deltaz;
    double distance;
    double gradelin;
    int mcc_id;
    int mccid_int;
    double rlmax;
    double rlmin;
    char *segside;
    int statusid;
    int streetid;
    int streetgroup;
    double start_lat;
    double start_lon;
    double end_lat;
    double end_lon;
} footpath;

typedef footpath footpath_t;

void footpathSkipHeaderLine(FILE *f);
char *readStringData(FILE *f);
footpath_t *readFootpath(FILE *f);
void printFootpathOutput(footpath_t *footpath, FILE *output);
void footpathFree(void *s);


#endif
