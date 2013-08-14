#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

bool empty(const char *s);
char **tokenize_string(char *, int *);
char *ltrim(const char *s);
char *rtrim(const char *s);
char *trim(const char *s);
