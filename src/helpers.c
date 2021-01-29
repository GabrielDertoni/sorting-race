#include <helpers.h>

void swap(elem_t *a, elem_t *b) {
    elem_t tmp = *a;
    *a = *b;
    *b = tmp;
}

int compare(elem_t *a, elem_t *b) {
    return (*a > *b) - (*b > *a);
}
