#ifndef _MERGE3_H
#define _MERGE3_H

#include <helpers.h>

void merge3(elem_t *start, elem_t *mid1, elem_t *mid2, elem_t *end);
void merge3Sort(elem_t *start, elem_t *end);
void merge3With(elem_t *start, elem_t *mid1, elem_t *mid2, elem_t *end, comp_t comp);
void merge3SortWith(elem_t *start, elem_t *end, comp_t comp);

#endif