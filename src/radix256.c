#include <stdlib.h>
#include <string.h>
#include <radix256.h>

void radix256Sort(unsigned int *start, unsigned int *end) {
    unsigned int n = end - start;
    unsigned int *vec = start;
    unsigned int *output = (unsigned int *)malloc(n * sizeof(unsigned int));
    unsigned int *count = (unsigned int *)malloc(256 * sizeof(int));
    for (int shift = 0; shift < 32; shift += 8) {
        // Zero the counts.
        memset(count, 0, 256 * sizeof(unsigned int));

        // Counts
        for (int i = 0; i < n; i++)
            count[(vec[i] >> shift) & 0xff]++;

        // Prefix sum
        for (int i = 1; i < 256; i++)
            count[i] += count[i - 1];

        // Replacing
        for (int i = n - 1; i >= 0; i--)
            output[--count[(vec[i] >> shift) & 0xff]] = vec[i];

        unsigned int *tmp = vec;
        vec = output;
        output = tmp;
    }
    free(output);
    free(count);
}

void radix256SignedSort(int *start, int *end) {
    int n = end - start;
    int *vec = start;
    int *output = (int *)malloc(n * sizeof(int));
    int *count = (int *)malloc(256 * sizeof(int));
    for (int shift = 0; shift < 32; shift += 8) {
        // Zero the counts.
        memset(count, 0, 256 * sizeof(int));

        // Counts
        for (int i = 0; i < n; i++)
            count[(vec[i] >> shift) & 0xff]++;

        // Prefix sum
        for (int i = 1; i < 256; i++)
            count[i] += count[i - 1];

        // Replacing
        for (int i = n - 1; i >= 0; i--)
            output[--count[(vec[i] >> shift) & 0xff]] = vec[i];

        int *tmp = vec;
        vec = output;
        output = tmp;
    }
    // One more operation required on the *inverted* signal bit.
    count[0] = count[1] = 0; // Zeroing
    // Counts
    for (int i = 0; i < n; i++)
        count[~(vec[i] >> 31) & 1]++;

    count[1] += count[0]; // Prefix sum

    // Replacing
    for (int i = n - 1; i >= 0; i--)
        output[--count[~(vec[i] >> 31) & 1]] = vec[i];

    memcpy(vec, output, n * sizeof(int));

    free(output);
    free(count);
}
