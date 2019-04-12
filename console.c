#include "console.h"

char *read_string(void)
{
	char *str = NULL;
	ssize_t buf = 0;

	if (getline(&str, &buf, stdin) != -1)
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

int split(char *str, char **tokens)
{

}
