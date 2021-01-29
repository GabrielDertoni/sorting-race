#include <stdlib.h>
#include <string.h>
#include <helpers.h>
#include <merge3.h>

#include <stdio.h>

void merge3(elem_t *start, elem_t *mid1, elem_t *mid2, elem_t *end) {
    elem_t *auxorig = (elem_t *)malloc((end - start) * sizeof(elem_t));
    elem_t *startPtr = start, *mid1Ptr = mid1, *mid2Ptr = mid2, *aux = auxorig;

    // Toma conta de juntar as partições, mesmo se startPtr == mid1
    while (mid1Ptr < mid2 && mid2Ptr < end)
        if (startPtr < mid1 && *startPtr < *mid1Ptr)
            if (*startPtr < *mid2Ptr)
                *(aux++) = *(startPtr++);
            else
                *(aux++) = *(mid2Ptr++);
        else
            if (*mid1Ptr < *mid2Ptr)
                *(aux++) = *(mid1Ptr++);
            else
                *(aux++) = *(mid2Ptr++);
    
    // Junta as partições restantes caso mid2Ptr == end
    // Merges left over partitions where mid2Ptr == end
    while (mid1Ptr < mid2 && startPtr < mid1)
        if (*startPtr < *mid1Ptr)
            *(aux++) = *(startPtr++);
        else
            *(aux++) = *(mid1Ptr++);
    
    // Junta as partições restantes caso mid1Ptr == mid2
    // Merges left over partitions where mid1Ptr == mid2
    while (mid2Ptr < end && startPtr < mid1)
        if (*startPtr < *mid2Ptr)
            *(aux++) = *(startPtr++);
        else
            *(aux++) = *(mid2Ptr++);
    
    while (startPtr < mid1) *(aux++) = *(startPtr++);
    while (mid1Ptr  < mid2) *(aux++) = *(mid1Ptr++);
    while (mid2Ptr  < end ) *(aux++) = *(mid2Ptr++);

    memcpy(start, auxorig, (end - start) * sizeof(elem_t));
    free(auxorig);
}

void merge3Sort(elem_t *start, elem_t *end) {
    if (start >= end - 1) return;
    // Add 2 at the end to garantee rounding up.
    elem_t *mid1 = start + (end - start + 2) / 3;
    elem_t *mid2 = start + (2 * (end - start) + 2) / 3;
    merge3Sort(start, mid1);
    merge3Sort(mid1, mid2);
    merge3Sort(mid2, end);
    merge3(start, mid1, mid2, end);
}

void merge3With(elem_t *start, elem_t *mid1, elem_t *mid2, elem_t *end, comp_t comp) {
    elem_t *auxorig = (elem_t *)malloc((end - start) * sizeof(elem_t));
    elem_t *startPtr = start, *mid1Ptr = mid1, *mid2Ptr = mid2, *aux = auxorig;

    // Toma conta de juntar as partições, mesmo se startPtr == mid1
    while (mid1Ptr < mid2 && mid2Ptr < end)
        if (startPtr < mid1 && comp(startPtr, mid1Ptr) < 0)
            if (comp(startPtr, mid2Ptr) < 0)
                *(aux++) = *(startPtr++);
            else
                *(aux++) = *(mid2Ptr++);
        else
            if (comp(mid1Ptr, mid2Ptr) < 0)
                *(aux++) = *(mid1Ptr++);
            else
                *(aux++) = *(mid2Ptr++);
    
    // Junta as partições restantes caso mid2Ptr == end
    // Merges left over partitions where mid2Ptr == end
    while (mid1Ptr < mid2 && startPtr < mid1)
        if (comp(startPtr, mid1Ptr) < 0)
            *(aux++) = *(startPtr++);
        else
            *(aux++) = *(mid1Ptr++);
    
    // Junta as partições restantes caso mid1Ptr == mid2
    // Merges left over partitions where mid1Ptr == mid2
    while (mid2Ptr < end && startPtr < mid1)
        if (comp(startPtr, mid2Ptr) < 0)
            *(aux++) = *(startPtr++);
        else
            *(aux++) = *(mid2Ptr++);
    
    while (startPtr < mid1) *(aux++) = *(startPtr++);
    while (mid1Ptr  < mid2) *(aux++) = *(mid1Ptr++);
    while (mid2Ptr  < end ) *(aux++) = *(mid2Ptr++);

    memcpy(start, auxorig, (end - start) * sizeof(elem_t));
    free(auxorig);
}

void merge3SortWith(elem_t *start, elem_t *end, comp_t comp) {
    if (start >= end - 1) return;
    // Add 2 at the end to garantee rounding up.
    elem_t *mid1 = start + (end - start + 2) / 3;
    elem_t *mid2 = start + (2 * (end - start) + 2) / 3;
    merge3SortWith(start, mid1, comp);
    merge3SortWith(mid1, mid2, comp);
    merge3SortWith(mid2, end, comp);
    merge3With(start, mid1, mid2, end, comp);
}
