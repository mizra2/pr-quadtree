#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"


// Create an empty list  and returns a pointer to that list
list_t
*make_empty_list() {
	list_t *list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->tail = NULL;
	return list;
}

// Adds a footpath struct to end of a linked list
// Inspired by Workshop 4 and listops.c from PPSAA
list_t
*appendFootpath(list_t *list, footpath_t *value) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->data = value;
	new->next = NULL;
	if (list->tail==NULL) {
		list->head = list->tail = new;
	} else {
		list->tail->next = new;
		list->tail = new;
	}
	return list;
}
// Reads a line, creates a struct and stores it into a footpath list
int
buildFootpathList(FILE *f, list_t *footpathList) {
	int n = 0;
	footpath_t *footpath;
	footpathSkipHeaderLine(f);
	while((footpath = readFootpath(f)) != NULL) {
        appendFootpath(footpathList, footpath);
        n++;
    }
    free(footpath);
	return n;
}

// Frees memory stored in the linked list
void freeFootpath(list_t *list) {
    assert(list != NULL);
    node_t *current = list->head;
    while (current) {
        node_t *tmp = current;
        current = current->next;
        footpathFree(tmp->data);
        free(tmp);
    }
    free(list);
}
