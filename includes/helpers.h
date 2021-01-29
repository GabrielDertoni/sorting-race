#ifndef _SORT_HELPERS_H
#define _SORT_HELPERS_H

typedef int elem_t;
typedef int (*comp_t)(elem_t*, elem_t*);

void swap(elem_t *a, elem_t *b);
int compare(elem_t *a, elem_t *b);

#endif
