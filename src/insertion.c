#include <helpers.h>
#include <insertion.h>

void insertionSort(elem_t *start, elem_t *end) {
    for (elem_t *a = start; a < end - 1; a++)
        for (elem_t *b = a + 1; b > start && *b < *(b - 1); b--)
            swap(b, b - 1);
}

void insertionSortWith(elem_t *start, elem_t *end, comp_t comp) {
    for (elem_t *a = start; a < end - 1; a++)
        for (elem_t *b = a + 1; b > start && comp(b, b - 1) < 0; b--)
            swap(b, b - 1);
}
