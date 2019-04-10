#ifndef CONSOLE
#define CONSOLE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 100

int read_line(void);
int split(char *str, char **tokens);

#endif
