#include "console.h"

char *keys[] = {
	"\027\094\091\065" // UpArrow
};
char *functions[] = {
	"exit",
	"cd"
};
int (*built_in[]) (const char **) = {
	&exit_term,
	&_cd
};
size_t size = MAX_COMMANDS + 2;
size_t size_keys = sizeof keys;
size_t func = sizeof functions;
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

int recognize_key(char *line, size_t pos) {
	line[pos + 1] = '\0';

	return strcmp(line, keys[0]);
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
		// if (c == '\033') {
		// 	fflush(stdin);
		// 	fflush(stdout);
		// 	lseek(STDIN_FILENO, 0, SEEK_END);
		// 	char *com = get_command();
		// 	fprintf(stdin, "%s", com);
		// 	fprintf(stdout, "%s", com);
		// 	continue;
		// }
		// printf("%c\n", c);
		buf[pos] = c;
		pos++;

		if (buf_size <= pos) {
			buf_size += BUF_SIZE;
			buf = realloc(buf, buf_size);
			if (buf == NULL)
				goto error;
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

char *split(char *str)
{
	int i, spaces = 0;
 	for (i = 0; i < strlen(str); i++)
 		if(str[i] == ' ')
 			spaces++;

 	printf("%d", spaces);
 	char *words[spaces + 2];
 	char *p = strtok(str, " ");
 	i = 0;

 	while (p != NULL) {
  		words[i++] = p;
  		p = strtok(NULL, " ");
	}

	printf("%s\n", words[0]);
	printf("%s\n", words[1]);

 	words[spaces + 1] = "\0";
 	return words;
}
