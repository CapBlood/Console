#include "console.h"

char *functions[] = {
	"exit",
	"cd"
};
int (*built_in[]) (const char **) = {
	&exit_term,
	&_cd
};
size_t size = MAX_COMMANDS + 2;
size_t func = sizeof functions / sizeof(char *);
char **commands;
size_t curr_command;
size_t top;
size_t bottom;
static int pos_buf;

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

void exit_term(const char **argv)
{
	exit(EXIT_SUCCESS);
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

int read_key(void)
{
	char buf[4];
	struct termios term_saved;
	if (tcgetattr(STDIN_FILENO, &term_saved) == -1) {
		perror("SuperTerm");
		return -1;
	}

	struct termios curr_term = term_saved;
	curr_term.c_lflag &= ~ICANON;
	curr_term.c_lflag |= ECHO;

	tcsetattr(STDIN_FILENO, TCSANOW, &curr_term);

	int pos = 0;
	char ch;
	while ((ch = getchar()) != EOF && pos != 4) {
		buf[pos] = ch;
		pos++;
	}

	if (strcmp(buf, UP_ARROW) == 0) {
		printf("\r");
		printf("%s", get_command());
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &term_saved);
	return 0;
}

char *read_string(void)
{
	int buf_size = BUF_SIZE;
	char *buf = malloc(buf_size);
	if (buf == NULL)
		goto error;

	char c;
	int pos = 0;
	pos_buf = 0;
	while ((c = getchar()) != '\n' && c != EOF) {
		buf[pos] = c;
		pos++;

		if (buf_size <= pos) {
			char *tmp;
			buf_size += BUF_SIZE;
			tmp = realloc(buf, buf_size);
			if (tmp != NULL)
				buf = tmp;
			else {
				free(buf);
				goto error;
			}
		}
	}
	buf[pos] = '\0';

	return buf;

error:
	perror("SuperTerm");
	return NULL;
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

char **split(char *str)
{
	int bufsize = TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens)
		goto error;

	token = strtok(str, TOK_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			char *tmp;
			bufsize += TOK_BUFSIZE;
			tmp = realloc(tokens, bufsize * sizeof(char*));
			if (tmp != NULL)
				tokens = tmp;
			else {
				free(tokens);
				goto error;
			}
		}

		token = strtok(NULL, TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;

error:
	perror("SuperTerm");
	return NULL;
}
