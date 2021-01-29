#ifndef _PARSING_H
#define _PARSING_H

#include <stdbool.h>

typedef struct {
    char *str;
    int size;
} segment_t;

char *nexttok(char *string);
bool isprefix(char *prefix, char *str);
char *after(segment_t seg);
char *fromseg(segment_t *seg);

segment_t mkseg(char *str);
segment_t *pbetween(char *str, char *start, char *end);
segment_t *puntil(char *str, char *stop);
segment_t *psome(char *str, char *chars);

/* Parses sequence of characters
 * Parameters:
 *  - str: string to parse on
 *  - seq: sequence to parse on str
 *
 */
char *pseq(char *str, char *seq);

int segcmp(segment_t a, segment_t b);

#endif
