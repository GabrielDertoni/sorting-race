#include <stdlib.h>
#include <helpers.h>
#include <cocktail.h>

void cocktailSort(elem_t *begin, elem_t *end) {
    while (begin < end) {
        for (elem_t *up = begin + 1; up < end; up++)
            if (*(up - 1) > *up)
                swap(up - 1, up);

        end--;
        for (elem_t *down = end - 2; down >= begin; down--)
            if (*down > *(down + 1))
                swap(down, down + 1);

        begin++;
    }
}

void cocktailSortWith(elem_t *begin, elem_t *end, comp_t comp) {
    while (begin < end) {
        for (elem_t *up = begin + 1; up < end; up++)
            if (comp(up - 1, up) > 0)
                swap(up - 1, up);

        end--;
        for (elem_t *down = end - 2; down >= begin; down--)
            if (comp(down, down + 1) > 0)
                swap(down, down + 1);

        begin++;
    }
}
