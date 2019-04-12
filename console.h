#ifndef CONSOLE
#define CONSOLE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char *read_string(void);
int split(char *str, char **tokens);
int start(char **argv);



#endif
