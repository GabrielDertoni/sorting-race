#ifndef _SORT_MERGE_H
#define _SORT_MERGE_H

#include <helpers.h>

void merge(elem_t *start, elem_t *mid, elem_t *end);
void mergeSort(elem_t *start, elem_t *end);
void mergeWith(elem_t *start, elem_t *mid, elem_t *end, comp_t comp);
void mergeSortWith(elem_t *start, elem_t *end, comp_t comp);

#endif
