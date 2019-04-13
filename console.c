#include "console.h"

char *functions[] = {
  "exit",
  "cd"
};
int (*built_in[]) (const char **)  = {
  &exit_term,
  &_cd
};
size_t size = MAX_COMMANDS + 2;
size_t func = sizeof functions;
char **commands;
size_t curr_command;
size_t top;
size_t bottom;

int init_buffer()
{
	commands = calloc(size - 1, sizeof(char *));
	if (commands == NULL) {
		perror("SuperTerm");
		return -1;
	}
	curr_command = 1;
	top = 1;
	bottom = 0;

	return 0;
}

void put_command(char *line)
{
	commands[top] = malloc(strlen(line) + 1);
	strcpy(commands[top], line);
	curr_command = top;
	top = (top + size + 1) % size;
	if (top == bottom)
		bottom = (bottom + size + 1) % size;
}

char *get_command(void)
{
	int curr = curr_command;
	curr_command = (curr_command + size - 1) % size;
	return (curr != bottom) ? commands[curr] : NULL;
}

void del_buffer(void) {
	for (int i = top; i != bottom; i = (i + size - 1) % size)
		free(commands[i]);
	free(commands);
}

int exit_term(const char **argv)
{
	return 0;
}

int _cd(const char **argv)
{
	if (argv[1] == NULL) {
		fprintf(stderr, "Enter the path\n");
		return -1;
	}

	if (chdir(argv[1]) == -1) {
		perror("SuperTerm");
		return -1;
	}

	return 1;
}

char *read_string(void)
{
	char *str = NULL;
	ssize_t buf = 0;

	if (getline(&str, &buf, stdin) == -1)
		perror("SuperTerm");

	return str;
}

int start(char **argv)
{
	int state;
	pid_t id = fork();

	if (id == 0) {
		if (execvp(argv[0], argv) == -1)
			goto error;
	} else if (id < 0) {
		goto error;
	} else {
		do {
			waitpid(id, &state, WUNTRACED);
		} while (!WIFEXITED(state) && !WIFSIGNALED(state));
	}

	return 1;

error:
	perror("SuperTerm");
	return -1;
}

int execute(char **argv)
{
	if (argv[0] == NULL)
		return -1;

	for (int i = 0; i < func; i++) {
		if (strcmp(argv[0], functions[i]) == 0)
			return built_in[i] (argv);
	}

	return start(argv);
}

int split(char *str, char **tokens)
{

}
