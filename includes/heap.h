#ifndef _SORT_HEAP_H
#define _SORT_HEAP_H

#include <helpers.h>

void makeMaxHeap(elem_t *root, elem_t *start, elem_t *end);
void heapSort(elem_t *start, elem_t *end);
void makeMaxHeapWith(elem_t *root, elem_t *start, elem_t *end, comp_t comp);
void heapSortWith(elem_t *start, elem_t *end, comp_t comp);

#endif
