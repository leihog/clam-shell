#include "strfunc.h"

bool empty(const char *str)
{
	if (!str || !strlen(str)) {
		return false;
	}

	while(*str) {
		if (!isspace(*str)) return false;
		str++;
	}
	return true;
}

char **tokenize_string(char *line, int *count)
{
	char *token;
	int size = 2; // a reasonable default
	char *tmpstr = strdup(line);

	char **args = malloc(sizeof(char *) * size + 1);
	if (!args)
		return args; // should be null

	*count = 0;
	while ((token = strsep(&tmpstr, " \t")) != NULL) {
		if (*count == size) { // need to reallocate memory
			size += 2;
			args = (char **)realloc(args, sizeof(char *) * size + 1);
			if (!args)
				return args; // should be null
		}

		args[(*count)++] = token;
	}
	args[*count] = NULL;

	return args;
}

char *rtrim(const char *s)
{
  while( isspace(*s) || !isprint(*s) ) ++s;
  return strdup(s);
}

char *ltrim(const char *s)
{
  char *r = strdup(s);
  if (r != NULL)
  {
    char *fr = r + strlen(s) - 1;
    while( (isspace(*fr) || !isprint(*fr) || *fr == 0) && fr >= r) --fr;
    *++fr = 0;
  }
  return r;
}

char *trim(const char *s)
{
  char *r = rtrim(s);
  char *f = ltrim(r);
  free(r);
  return f;
}
