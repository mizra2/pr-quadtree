#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"


/* Reads the string data contained in a CSV file
          Checking for empty fields
Special Thanks / Credit to "Anh Vo" for assistance 
    on how to process string data */

char *readStringData(FILE *f) {
    char buff[MAX_LINE_LENGTH + 1];
    // Get the first character of the string data
    buff[0] = getc(f);
    // if that character is a ",", then we know that field is an empty field
    if (buff[0] == ',') {
        return strdup("\0");
    }
    // If the first character is a "\"", then process and read string till the next
    // "\"", including any commas part of the string
    if (buff[0] == '\"') {
        fscanf(f, "%[^\"]\",", buff);
    } 
    // Else just scan the string normally
    else {
        fscanf(f, "%[^,],", buff+1);
    }
    return strdup(buff);
}
// Reads 1 footpath
footpath_t *readFootpath(FILE *f) {
    footpath_t *footpath = NULL;
    int footpath_id;
    char *address;
    char *clue_sa;
    char *asset_type;
    char *segside;
    double start_lat, start_lon, end_lat, end_lon, rlmax, rlmin, deltaz, distance, gradelin, mcc_id, mccid_int, statusid, streetid, streetgroup;

    if (fscanf(f, "%d,", &footpath_id) != 1) return footpath;
    footpath = malloc(sizeof(*footpath));

    footpath->footpath_id = footpath_id;

    address = readStringData(f);
    footpath->address = address;

    clue_sa = readStringData(f);
    footpath->clue_sa = clue_sa;

    asset_type = readStringData(f);
    footpath->asset_type = asset_type;

    fscanf(f, "%lf,", &deltaz);
    footpath->deltaz = deltaz;

    fscanf(f, "%lf,", &distance);
    footpath->distance = distance;

    fscanf(f, "%lf,", &gradelin);
    footpath->gradelin = gradelin;

    fscanf(f, "%lf,", &mcc_id);
    footpath->mcc_id = (int) mcc_id;

    fscanf(f, "%lf,", &mccid_int);
    footpath->mccid_int = (int) mccid_int;

    fscanf(f, "%lf,", &rlmax);
    footpath->rlmax = rlmax;

    fscanf(f, "%lf,", &rlmin);
    footpath->rlmin = rlmin;

    segside = readStringData(f);
    footpath->segside = segside;

    fscanf(f, "%lf,", &statusid);
    footpath->statusid = (int) statusid;

    fscanf(f, "%lf,", &streetid);
    footpath->streetid = (int) streetid;

    fscanf(f, "%lf,", &streetgroup);
    footpath->streetgroup = (int) streetgroup;

    fscanf(f, "%lf,", &start_lat);
    footpath->start_lat = start_lat;

    fscanf(f, "%lf,", &start_lon);
    footpath->start_lon = start_lon;

    fscanf(f, "%lf,", &end_lat);
    footpath->end_lat = end_lat;

    fscanf(f, "%lf\n", &end_lon);
    footpath->end_lon = end_lon;
    return footpath;
}

void footpathSkipHeaderLine(FILE *f) {
    while (fgetc(f) != '\n');
}
// Process footpath data
void printFootpathOutput(footpath_t *footpath, FILE *output) {
    fprintf(output, "--> footpath_id: %d || address: %s || clue_sa: %s || asset_type: %s || deltaz: %.2f || distance: %.2f || grade1in: %.1f || mcc_id: %d || mccid_int: %d || rlmax: %.2f || rlmin: %.2f || segside: %s || statusid: %d || streetid: %d || street_group: %d || start_lat: %lf || start_lon: %lf || end_lat: %lf || end_lon: %lf || \n", 
    footpath->footpath_id, footpath->address, footpath->clue_sa, footpath->asset_type, footpath->deltaz, footpath->distance, footpath->gradelin, footpath->mcc_id, footpath->mccid_int, footpath->rlmax, footpath->rlmin, 
    footpath->segside, footpath->statusid, footpath->streetid, footpath->streetgroup, footpath->start_lat, footpath->start_lon, footpath->end_lat, footpath->end_lon);
}
// Free footpath and its data
void footpathFree(void *s) {
	free(((footpath_t *)s)->address);
    free(((footpath_t *)s)->segside);
    free(((footpath_t *)s)->asset_type);
    free(((footpath_t *)s)->clue_sa);
	free(s);
}

