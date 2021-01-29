#include <helpers.h>
#include <heap.h>

// Heapify
void makeMaxHeap(elem_t *root, elem_t *start, elem_t *end) {
    elem_t *largest = root;
    elem_t *left = start + 1 + (root - start) * 2;
    elem_t *right = left + 1;

    if (left < end && *left > *largest)
        largest = left;

    if (right < end && *right > *largest)
        largest = right;

    if (largest != root) {
        swap(root, largest);
        makeMaxHeap(largest, start, end);
    }
}

void heapSort(elem_t *start, elem_t *end) {
    // Construct the initial heap.
    for (int i = (end - start) / 2 - 1; i >= 0; i--)
        makeMaxHeap(start + i, start, end);

    // Heap sort.
    for (; start < end; swap(start, --end))
        makeMaxHeap(start, start, end);
}

void makeMaxHeapWith(elem_t *root, elem_t *start, elem_t *end, comp_t comp) {
    elem_t *largest = root;
    elem_t *left = start + 1 + (root - start) * 2;
    elem_t *right = left + 1;

    if (left < end && comp(left, largest) > 0)
        largest = left;

    if (right < end && comp(right, largest) > 0)
        largest = right;

    if (largest != root) {
        swap(root, largest);
        makeMaxHeap(largest, start, end);
    }
}

void heapSortWith(elem_t *start, elem_t *end, comp_t comp) {
    // Construct the initial heap.
    for (int i = (end - start) / 2 - 1; i >= 0; i--)
        makeMaxHeapWith(start + i, start, end, comp);

    // Heap sort.
    for (swap(start, --end); start < end; swap(start, --end))
        makeMaxHeapWith(start, start, end, comp);
}
