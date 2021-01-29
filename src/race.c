#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <helpers.h>
#include <parsing.h>
#include <argsparse.h>
#include <colors.h>

#include <heap.h>
#include <bubble.h>
#include <cocktail.h>
#include <insertion.h>
#include <quick.h>
#include <merge.h>
#include <merge3.h>
#include <radix256.h>

#define MAIN_DEBUG
#include <debug.h>

#define time_fn(func) ({ \
    clock_t __st = clock();\
    func;\
    clock() - __st;\
})
#define avg_time_fn(reset, func, repeats) ({ \
    long long __avg = 0;\
    for (int __i = 0; __i < repeats; __i++) {\
        reset;\
        clock_t __st = clock();\
        func;\
        __avg += clock() - __st;\
    }\
    __avg / repeats;\
})

void sortTimes(double *times, char *names[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j > 0 && ((times[j] < times[j - 1] && times[j] >= 0) || times[j - 1] < 0); j--) {
            double t1 = times[j];
            times[j] = times[j - 1];
            times[j - 1] = t1;
            char *t2 = names[j];
            names[j] = names[j - 1];
            names[j - 1] = t2;
        }
}

bool check(int *start, int *end) {
    for (; start < end - 1; start++) 
        if (*start > *(start + 1))
            return false;

    return true;
}

int main(int argc, char *argv[]) {
    char *algorithms[20];
    for (int i = 0; i < 20; i++)
        algorithms[i] = (char *)malloc(20*sizeof(char));

    int n = 100, r = 1, nalgos;
    bool debug = false, input = false;
    int argerr = scanarg(argc, argv,
                         "(algorithms:%s)... [-n %d] [-r %d] [--debug %b] [--input %b]",
                         &algorithms, &n, &r, &debug, &input);

    if (argerr) {
        pargerr(argerr);
        exit(EXIT_FAILURE);
    }

    printf("Racing: ");
    for (nalgos = 0; nalgos < 20 && algorithms[nalgos]; nalgos++)
        printf("%s ", algorithms[nalgos]);

    printf("with %d elements. Average of %d\n", n, r);

    srand(time(NULL) * n);
    int *vec = (int *)malloc(n * sizeof(int));
    int *copy = (int *)malloc(n * sizeof(int));
    if (input)
        for (int i = 0; i < n; i++)
            scanf(" %d", &vec[i]);
    else
        for (int i = 0; i < n; i++)
            if (debug) vec[i] = rand() % (3 * n);
            else vec[i] = rand();

    double times[nalgos];
    for (int i = 0; i < nalgos; i++) {
        printf("%s...", algorithms[i]);
        fflush(stdout);
        memcpy(copy, vec, n * sizeof(int));
        if (!strcmp("heap", algorithms[i]))
            times[i] = (double)avg_time_fn(memcpy(copy, vec, n * sizeof(int)),
                                           heapSort(copy, copy + n),
                                           r) / (double)CLOCKS_PER_SEC;

        else if (!strcmp("insertion", algorithms[i]))
            times[i] = (double)avg_time_fn(memcpy(copy, vec, n * sizeof(int)),
                                           insertionSort(copy, copy + n),
                                           r) / (double)CLOCKS_PER_SEC;

        else if (!strcmp("bubble", algorithms[i]))
            times[i] = (double)avg_time_fn(memcpy(copy, vec, n * sizeof(int)),
                                           bubbleSort(copy, copy + n),
                                           r) / (double)CLOCKS_PER_SEC;

        else if (!strcmp("cocktail", algorithms[i]))
            times[i] = (double)avg_time_fn(memcpy(copy, vec, n * sizeof(int)),
                                           cocktailSort(copy, copy + n),
                                           r) / (double)CLOCKS_PER_SEC;

        else if (!strcmp("quick", algorithms[i]))
            times[i] = (double)avg_time_fn(memcpy(copy, vec, n * sizeof(int)),
                                           quickSort(copy, copy + n),
                                           r) / (double)CLOCKS_PER_SEC;

        else if (!strcmp("merge", algorithms[i]))
            times[i] = (double)avg_time_fn(memcpy(copy, vec, n * sizeof(int)),
                                           mergeSort(copy, copy + n),
                                           r) / (double)CLOCKS_PER_SEC;

        else if (!strcmp("merge3", algorithms[i]))
            times[i] = (double)avg_time_fn(memcpy(copy, vec, n * sizeof(int)),
                                           merge3SortWith(copy, copy + n, compare),
                                           r) / (double)CLOCKS_PER_SEC;

        else if (!strcmp("radix256", algorithms[i]))
            times[i] = (double)avg_time_fn(memcpy(copy, vec, n * sizeof(int)),
                                           radix256SignedSort(copy, copy + n),
                                           r) / (double)CLOCKS_PER_SEC;

        else {
            printf(CLEAR RED BOLD "\rSort '%s' not found and/or not implemented, ignoring." RESET "\n", algorithms[i]);
            times[i] = -1;
            continue;
        }

        printf(CLEAR "\rVerifying...");
        fflush(stdout);
        if (!check(copy, copy + n)) {
            printf("\n" RED BOLD "ERROR: %s did not sort correctly! Continuing." RESET "\n", algorithms[i]);
            if (debug) {
                printf("[ %d", copy[0]);
                for (int i = 1; i < n; i++) printf(", %d", copy[i]);
                printf(" ]\n");
            }
            times[i] = -1;
        } else {
            printf(CLEAR GREEN "\r%s DONE!  \n" RESET, algorithms[i]);
            printf("Time of %s: %.17lfs\n", algorithms[i], times[i]);
        }
    }

    sortTimes(times, algorithms, nalgos);
    printf("\n" UNDERLINE BG_BLUE WHITE BOLD "%-57s" RESET "\n", "SUMMARY:");
    for (int i = 0; i < nalgos; i++)
        if (times[i] >= 0)
            if (i % 2 == 0)
                printf(BG_BLACK WHITE BOLD "Time of %-15s %32.17lfs" RESET "\n", algorithms[i], times[i]);
            else
                printf(BG_WHITE BLACK "Time of %-15s %32.17lfs" RESET "\n", algorithms[i], times[i]);
        else
            printf(BG_RED WHITE BOLD "%-49s FAILED!" RESET "\n", algorithms[i]);

    printf("\n");

    for (int i = 0; i < 20; i++)
        free(algorithms[i]);

    free(vec);
    free(copy);

    return EXIT_SUCCESS;
}
