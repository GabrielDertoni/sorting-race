#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <parsing.h>
#include <debug.h>

#define ALLOC_SEG(rstr, rsize) ({ \
    segment_t *rslt = (segment_t *)malloc(sizeof(segment_t)); \
    rslt->str = rstr; \
    rslt->size = rsize; \
    rslt; \
})

char *nexttok(char *string) {
    char *c;
    for (c = string; strchr(" \n\r\t", *c) && *c != '\0'; c++);
    return c;
}

bool isprefix(char *prefix, char *str) {
    if (!*prefix) return true;
    if (!*str || *str != *prefix) return false;
    return isprefix(prefix + 1, str + 1);
}

char *after(segment_t seg) {
    return seg.str + seg.size;
}

char *fromseg(segment_t *seg) {
    char *str = (char *)calloc(seg->size, sizeof(char));
    memcpy(str, seg->str, seg->size * sizeof(char));
    return str;
}


segment_t mkseg(char *str) {
    segment_t seg = {
        .str = str,
        .size = strlen(str)
    };
    return seg;
}

segment_t *pbetween(char *str, char *start, char *end) {
    char *rstr;
    int rsize = 0;
    if ((rstr = pseq(str, start))) {
        char *tail = rstr;
        for (segment_t *tmp; *tail && !pseq(tail, end); tail++, rsize++)
            // Allow nested segments. i.e. between (): "((hello))" -> "(hello)"
            if (pseq(tail, start) && (tmp = pbetween(tail, start, end))) {
                rsize += after(*tmp) - tail + strlen(end) - 1;
                tail = after(*tmp) + strlen(end) - 1;
                free(tmp);
            }

        if (!*tail) return NULL;
        return ALLOC_SEG(rstr, rsize);
    }
    return NULL;
}

segment_t *puntil(char *str, char *stop) {
    segment_t *until = (segment_t *)malloc(sizeof(segment_t));
    until->str = str;
    until->size = 0;
    while(*str && !strchr(stop, *(str++)))
        until->size++;

    return until;
}

segment_t *psome(char *str, char *chars) {
    segment_t *some = (segment_t *)malloc(sizeof(segment_t));
    some->str = str;
    some->size = 0;
    while(*str && strchr(chars, *(str++)))
        some->size++;

    return some;
}

char *pseq(char *str, char *seq) {
    for (; *str && *seq && *str == *seq; seq++, str++);
    if (!*seq) return str;
    return NULL;
}

int segcmp(segment_t a, segment_t b) {
    int i;
    for (i = 0; i < a.size && i < b.size && a.str[i] == b.str[i]; i++);
    if (i == a.size && i == b.size) return 0;
    if (i == b.size || a.str[i] > b.str[i]) return 1;
    if (i == a.size || a.str[i] < b.str[i]) return -1;
    return -1;
}

