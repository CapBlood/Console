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
#define UP_ARROW "\027\094\091\065"

// Built-in functions
int exit_term(char **argv); // Exit from terminal
int _cd(char **argv); // Change dir to certain path

// Functions for terminal
char *read_string(void); // Read a string
char *get_command(void); // Get a command from buffer
void put_command(char *line); // Put a command to buffer
char **split(char *str); // Split a string by spaces
int start(char **argv); // Starting a proccess
int execute(char **argv); // Parse a command(built-in or from environment)
int init_buffer(void); // Init a buffer for commands
int init_raw(void); // Init raw mode for console
int reset_raw(void); // Disable raw mode for console
void kill_child(int type); // Kill a last child proccess
void del_buffer(void); // Delete a buffer

// Info for terminal
extern int (*built_in[]) (char **);
extern char *functions[];
extern size_t func;
extern char **commands;
extern size_t curr_command;
extern size_t top;

#endif
