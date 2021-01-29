#include <helpers.h>
#include <quick.h>

void quickSort(elem_t *start, elem_t *end) {
    if (start >= end - 1) return;
    elem_t *p1 = start + 1, *p2 = end - 1, *pivot = start;

    while (p1 < p2)
        if (*p1 >= *pivot && *p2 < *pivot)
            swap(p1++, p2--);
        else if (*p1 < *pivot) p1++;
        else if (*p2 >= *pivot) p2--;

    if (*p2 < *pivot) swap(p2, pivot);
    else swap(--p2, pivot);

    quickSort(start, p2);
    quickSort(p2 + 1, end);
}

void quickSortWith(elem_t *start, elem_t *end, comp_t comp) {
    if (start >= end) return;
    elem_t *p1 = start + 1, *p2 = end - 1, *pivot = start;
    while (p1 < p2)
        if (comp(p1, pivot) >= 0 && comp(p2, pivot) < 0 )
            swap(p1++, p2--);
        else if (comp(p1, pivot) < 0) p1++;
        else if (comp(p2, pivot) >= 0) p2--;

    if (comp(p2, pivot) < 0) swap(p2, pivot);
    else swap(--p2, pivot);

    quickSort(start, p2);
    quickSort(p2 + 1, end);
}
