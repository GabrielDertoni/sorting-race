#ifndef _ARGSPARSE_H
#define _ARGSPARSE_H

#include <parsing.h>

#define FORMAT_CHARS "idlfpx"

#define ERROR_FORMAT     -1
#define ERROR_POS_AFTER  -2
#define ERROR_MPOS_AFTER -3
#define ERROR_OPT_NO_TAG -4
#define ERROR_MULTI_BOOL -5

typedef enum {
    POSITIONAL,
    MULTI_POSITIONAL,
    OPTIONAL,
    MULTI_OPTIONAL
} argtype_t;

typedef enum {
    ERROR_NO_TYPE,
    INT,
    LONG,
    LONG_LONG,
    SHORT,
    CHAR,
    FLOAT,
    DOUBLE,
    BOOL,
    STRING
} dtype_t;

typedef struct {
    argtype_t type;
    dtype_t dtype;
    segment_t *name;
} arg_t;


int vscanarg(int argc, char *argv[], char *format, va_list ap);
int scanarg(int argc, char *argv[], char *format, ...);
int find_arg(char *argname, int n, int argc, char *argv[]);
void pargerr(int error);

#endif
