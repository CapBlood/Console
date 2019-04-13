#ifndef CONSOLE
#define CONSOLE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAX_COMMANDS 10

// Built-in functions
int exit_term(const char **argv); // Exit from terminal
int _cd(const char **argv); // Change dir to certain path

// Functions for terminal
char *read_string(void); // Read a string
char *get_command(void); // Get a command from buffer
void put_command(char *line); // Put a command to buffer
int split(char *str, char **tokens); // Split a string by spaces
int start(char **argv); // Starting a proccess
int execute(char **argv); // Parse a command(built-in or from environment)
int init_buffer(void); // Init a buffer for commands

// Info for terminal
extern int (*built_in[]) (const char **);
extern char *functions[];
extern size_t func;
extern char **commands;
extern size_t curr_command;
extern size_t top;

#endif
