#include "console.h"

int read_line(void)
{
	char *str = NULL;
	ssize_t buf = 0;
	
	if (getline(&str, &buf, stdin) != -1) {
		perror("I/O error");
		return -1;
	}

	return 0;
}

int split(char *str, char **tokens)
{

}
