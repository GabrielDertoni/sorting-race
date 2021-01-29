#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <parsing.h>
#include <argsparse.h>
#include <debug.h>

int find_arg(char *argname, int n, int argc, char *argv[]) {
    for (int i = 0; i < argc; i++)
        if (!strcmp(argv[i], argname) && i + n < argc)
            return i;

    return -1;
}

int parg(dtype_t dtype, char *str, void *apptr) {
    switch (dtype) {
        case ERROR_NO_TYPE:
            return 1;

        case INT:
            *(int *)apptr = atoi(str);
            break;

        case LONG:
            *(long *)apptr = atol(str);
            break;

        case LONG_LONG:
            *(long long *)apptr = atoll(str);
            break;

        case SHORT:
            *(short *)apptr = atoi(str);
            break;

        case CHAR:
            *(char *)apptr = *str;
            break;

        case FLOAT:
            *(float *)apptr = (float)atof(str);
            break;

        case DOUBLE:
            *(double *)apptr = atof(str);
            break;

        case BOOL:
            if (!strcmp("true", str))
                *(bool *)apptr = true;
            else
                *(bool *)apptr = false;
            break;

        case STRING:
            strcpy((char *)apptr, str);
            break;
    }
    return 1;
}

char *typename(dtype_t dtype) {
    switch (dtype) {
        case ERROR_NO_TYPE:
            return "nothing";

        case INT:
        case LONG:
        case LONG_LONG:
        case SHORT:
            return "int";

        case CHAR:
            return "char";

        case FLOAT:
        case DOUBLE:
            return "float";

        case BOOL:
            return "bool";

        case STRING:
            return "string";

        default:
            return "any";
    }
}

void printUsageQuit(char *progname, arg_t **args, int nargs, int status) {
    char *name = NULL;
    printf("Usage: %s ", progname);
    for (int i = 0; i < nargs; i++) {
        name = fromseg(args[i]->name);
        if (args[i]->type == POSITIONAL)
            printf("%s ", name);
        else if (args[i]->type == MULTI_POSITIONAL)
            printf("%s... ", name);
        else if (args[i]->type == OPTIONAL)
            printf("[%s %s] ", name, typename(args[i]->dtype));
        else if (args[i]->type == MULTI_OPTIONAL)
            printf("[%s %s]... ", name, typename(args[i]->dtype));

        if (name) {
            free(name);
            name = NULL;
        }
    }
    printf("\n\n");

    if (nargs > 0 && args[0]->type <= MULTI_POSITIONAL)
        printf("POSITIONAL:\n");

    int i;
    for (i = 0; i < nargs && args[i]->type <= MULTI_POSITIONAL; i++) {
        name = fromseg(args[i]->name);
        char *many = "";
        if (args[i]->type == MULTI_POSITIONAL)
            many = " (1 or more)";

        printf("%s\t%s%s\n", name, typename(args[i]->dtype), many);

        if (name) {
            free(name);
            name = NULL;
        }
    }

    if (nargs > i && args[i]->type >= OPTIONAL)
        printf("%sOPTIONAL:\n", i > 0 ? "\n" : "");

    for (; i < nargs; i++) {
        name = fromseg(args[i]->name);
        char *many = "";
        if (args[i]->type == MULTI_OPTIONAL)
            many = " (0 or more)";

        printf("%s\t%s%s\n", name, typename(args[i]->dtype), many);

        if (name) {
            free(name);
            name = NULL;
        }
    }

    exit(status);
}

int readPositional(int argc, char *argv[], dtype_t dtype, void *apptr) {
    if (argc <= 0) return 0;
    return parg(dtype, argv[0], apptr);
}

int readMultiPositional(int argc, char *argv[], dtype_t dtype, void **apptr) {
    int i;
    for (i = 0; i < argc && *argv[i] != '-'; i++)
        readPositional(argc - i, argv + i, dtype, apptr[i]);

    free(apptr[i]);
    apptr[i] = NULL; // Null terminate the list.
    return i;
}

int readOptional(int argc, char *argv[], char *name, dtype_t dtype, void *apptr) {
    int apos;
    if (dtype == BOOL) {
        if ((apos = find_arg(name, 0, argc, argv)) >= 0)
            *(bool *)apptr = true;

        return 1;
    }

    if ((apos = find_arg(name, 1, argc, argv)) >= 0)
        parg(dtype, argv[apos + 1], apptr);
    else return 0;
    return 1;
}

int readMultOptional(int argc, char *argv[], char *name, dtype_t dtype, void **apptr) {
    int i, apos = 0;
    for (i = 0; (apos = find_arg(name, 1, argc - apos, argv + apos)) >= 0; i++, apos++)
        parg(dtype, argv[apos + 1], apptr[i]);

    free(apptr[i]);
    apptr[i] = NULL;
    return i;
}

dtype_t ptype(char *str) {
    if (isprefix("%d", str)) return INT;
    if (isprefix("%lld", str)) return LONG_LONG;
    if (isprefix("%ld", str)) return LONG;
    if (isprefix("%hd", str)) return SHORT;
    if (isprefix("%c", str)) return CHAR;
    if (isprefix("%f", str)) return FLOAT;
    if (isprefix("%lf", str)) return DOUBLE;
    if (isprefix("%b", str)) return BOOL;
    if (isprefix("%s", str)) return STRING;
    return ERROR_NO_TYPE;
}

arg_t *pposarg(char *str, char **next) {
    arg_t *arg = (arg_t *)malloc(sizeof(arg_t));
    segment_t *parenthesis = NULL, *dtype = NULL;
    if (!(parenthesis = pbetween(str, "(", ")")         ) ||
        !(arg->name   = puntil(parenthesis->str, ":")   ) ||
        !(dtype       = puntil(after(*arg->name) + 1, "")) ||
        !(arg->dtype  = ptype(dtype->str))              ) {
        // If something fails, free arg and return NULL.
        free(arg);
        arg = NULL;
    } else if ((*next = pseq(after(*parenthesis) + 1, "..."))) {
        arg->type = MULTI_POSITIONAL;
    } else {
        arg->type = POSITIONAL;
        *next = after(*parenthesis) + 1;
    }

    if (parenthesis) free(parenthesis);
    if (dtype)       free(dtype);

    return arg;
}

arg_t *poptarg(char *str, char **next) {
    arg_t *arg = (arg_t *)malloc(sizeof(arg_t));
    segment_t *brackets, *dtype;
    if (!(brackets   = pbetween(str, "[", "]")            ) ||
        !(arg->name  = puntil(nexttok(brackets->str), " ")) ||
        !(dtype      = puntil(after(*arg->name) + 1, "")  ) ||
        !(arg->dtype = ptype(dtype->str))                 ) {
        free(arg);
        arg = NULL;
    } else if ((*next = pseq(after(*brackets) + 1, "...")))
        arg->type = MULTI_OPTIONAL;
    else {
        *next = after(*brackets) + 1;
        arg->type = OPTIONAL;
    }

    if (brackets) free(brackets);
    if (dtype)    free(dtype);

    return arg;
}

int vscanarg(int argc, char *argv[], char *format, va_list ap) {
    // Argument parsing.
    bool ended_pos, ended_multi_pos = false;
    char *ptr, *next = format;
    int nargs = 1, error = 0, allocated = 1;
    arg_t **args = (arg_t **)malloc(allocated * sizeof(arg_t *));
    for (nargs = 0; *next && !error; nargs++) {
        if (nargs >= allocated)
            args = (arg_t **)realloc(args, (allocated *= 2) * sizeof(arg_t *));

        ptr = nexttok(next);
        if (!((args[nargs] = pposarg(ptr, &next)) ||
              (args[nargs] = poptarg(ptr, &next)) ))
            error = ERROR_FORMAT;

        if (args[nargs]->type != POSITIONAL && !ended_pos)
            ended_pos = true;
        else if (args[nargs]->type == POSITIONAL && ended_pos)
            error = ERROR_POS_AFTER;

        if (args[nargs]->type != MULTI_POSITIONAL && ended_pos)
            ended_multi_pos = true;
        else if (args[nargs]->type == MULTI_POSITIONAL && ended_multi_pos)
            error = ERROR_MPOS_AFTER;

        if (args[nargs]->type >= OPTIONAL && *(args[nargs]->name->str) != '-')
            error = ERROR_OPT_NO_TAG;

        if (args[nargs]->type == MULTI_OPTIONAL && args[nargs]->dtype == BOOL)
            error = ERROR_MULTI_BOOL;
    }

    if (error) {
        while (nargs) free(args[--nargs]);
        free(args);
        return error;
    }
    // Argument reading.
    if (find_arg("--help", 0, argc, argv) > 0 || find_arg("-h", 0, argc, argv) > 0)
        printUsageQuit(argv[0], args, nargs, EXIT_SUCCESS);

    char *name = NULL;
    for (int i = 0, j = 1; i < nargs; i++) {
        void *apptr = va_arg(ap, void *);
        if (args[i]->type == POSITIONAL)
            if (readPositional(argc - j, argv + j, args[i]->dtype, apptr))
                j++;
            else
                printUsageQuit(argv[0], args, nargs, EXIT_FAILURE);

        else if (args[i]->type == MULTI_POSITIONAL) {
            int nr;
            nr = readMultiPositional(argc - j, argv + j, args[i]->dtype, (void **)apptr);
            if (nr) j += nr;
            else printUsageQuit(argv[0], args, nargs, EXIT_FAILURE);
        } else if (args[i]->type == OPTIONAL)
            readOptional(argc - j, argv + j,
                         name = fromseg(args[i]->name),
                         args[i]->dtype,
                         apptr);

        else if (args[i]->type == MULTI_OPTIONAL)
            readMultOptional(argc - j, argv + j,
                             name = fromseg(args[i]->name),
                             args[i]->dtype,
                             (void **)apptr);
        if (name) {
            free(name);
            name = NULL;
        }
    }
    while (nargs) {
        --nargs;
        free(args[nargs]->name);
        free(args[nargs]);
    }
    free(args);

    return EXIT_SUCCESS;
}

int scanarg(int argc, char *argv[], char *format, ...) {
    va_list ap;
    va_start(ap, format);
    int retr = vscanarg(argc, argv, format, ap);
    va_end(ap);
    return retr;
}

void pargerr(int error) {
    char *msg;
    switch (error) {
        case ERROR_FORMAT:
            msg = "Bad formatting.";
            break;

        case ERROR_POS_AFTER:
            msg = "Positional argument after multi positional or optional";
            break;

        case ERROR_MPOS_AFTER:
            msg = "Multi positional after optional.";
            break;

        case ERROR_OPT_NO_TAG:
            msg = "Optional argument name should start with tag.";
            break;

        case ERROR_MULTI_BOOL:
            msg = "Boolean arguments must be positional or optional, not multi optional.";

        default:
            msg = "Unknown.";
    }
    fprintf(stderr, "ERROR: %s\n", msg);
}
