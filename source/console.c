#include "console.h"

char *functions[] = {
	"exit",
	"cd"
};
int (*built_in[]) (char **) = {
	&exit_term,
	&_cd
};
struct termios term_saved;
size_t size = MAX_COMMANDS + 2;
size_t func = sizeof functions / sizeof(char *);
char **commands;
size_t curr_command;
size_t top;
size_t bottom;
static int pos_buf;
static pid_t last_id;

int init_buffer()
{
	commands = (char **) calloc(size - 1, sizeof(char *));
	if (commands == NULL) {
		perror("SuperTerm");
		return -1;
	}
	curr_command = 1;
	top = 1;
	bottom = 0;

	return 0;
}

int init_raw(void)
{
	struct termios term;

	if (tcgetattr(STDIN_FILENO, &term_saved) == 1)
		goto error;

	term = term_saved;
	term.c_lflag &= ~ICANON;

	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		goto error;

	return 0;
error:
	perror("SuperTerm");
	return -1;
}

int reset_raw(void)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term_saved) == -1) {
		perror("SuperTerm");
		return -1;
	}

	return 0;
}

void put_command(char *line)
{
	commands[top] = (char *) malloc(strlen(line) + 1);
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

void del_buffer(void)
{
	for (int i = top; i != bottom; i = (i + size - 1) % size)
		free(commands[i]);
	free(commands);
}

int exit_term(char **argv)
{
	exit(EXIT_SUCCESS);
}

int _cd(char **argv)
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

void kill_child(int type)
{
	if (kill(last_id, type) == -1)
		perror("SuperTerm");
}

char *read_string(void)
{
	int buf_size = BUF_SIZE;
	char *buf = (char *) malloc(buf_size);
	char c;
	int pos = 0;
	char *tmp;

	if (buf == NULL)
		goto error;

	pos_buf = 0;
	while ((c = getchar()) != '\n' && c != EOF) {
		buf[pos] = c;
		pos++;

		if (buf_size <= pos) {
			buf_size += BUF_SIZE;
			tmp = (char *) realloc(buf, buf_size);
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
	int back_mode = 0;
	int state;
	pid_t id = fork();

	char **ptr = argv;
	for (; *(ptr + 1) != NULL; ptr++);
	if (strcmp(*ptr, "&") == 0) {
		*ptr = 0;
		back_mode = 1;
	}

	if (id == 0) {
		if (execvp(argv[0], argv) == -1)
			goto error;
	} else if (id < 0) {
		goto error;
	} else {
		struct sigaction act;
		struct sigaction prev;
		act.sa_handler = kill_child;
		sigemptyset(&act.sa_flags);
		act.sa_flags = 0;
		act.sa_flags |= SA_RESTART;
		act.sa_flags |= SA_RESETHAND;
		last_id = id;
		sigaction(SIGINT, &act, &prev);

		if (back_mode == 1)
			return 1;

		do {
			waitpid(id, &state, WUNTRACED);
		} while (!WIFEXITED(state) && !WIFSIGNALED(state));

		prev.sa_handler = SIG_DFL;
		sigaction(SIGINT, &prev, 0);
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
	char **tokens = (char **) malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens)
		goto error;

	token = strtok(str, TOK_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			char **tmp;
			bufsize += TOK_BUFSIZE;
			tmp = (char **) realloc(tokens, bufsize * sizeof(char*));
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
