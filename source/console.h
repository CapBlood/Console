#ifndef CONSOLE
#define CONSOLE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <termios.h>

#define MAX_COMMANDS 10
#define BUF_SIZE 100
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"

// Built-in functions
int exit_term(char **argv); // Exit from terminal
int _cd(char **argv); // Change dir to certain path

// Functions for terminal
char *read_string(void); // Read a string
char **split(char *str); // Split a string by spaces
int start(char **argv); // Starting a proccess
int execute(char **argv); // Parse a command(built-in or from environment)
void kill_child(int type); // Kill a last child proccess

// Info for terminal
extern int (*built_in[]) (char **);
extern char *functions[];
extern size_t func;

#endif
