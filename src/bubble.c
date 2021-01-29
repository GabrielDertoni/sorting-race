#include <helpers.h>
#include <bubble.h>

void bubbleSort(elem_t *start, elem_t *end) {
    for (elem_t *a = end - 1; a > start; a--)
        for (elem_t *b = start; b < a; b++)
            if (*b > *(b + 1))
                swap(b, b + 1);
}

void bubbleSortWith(elem_t *start, elem_t *end, comp_t comp) {
    for (elem_t *a = end - 1; a > start; a--)
        for (elem_t *b = start; b < a; b++)
            if (comp(b, b + 1) > 0)
                swap(b, b + 1);
}
