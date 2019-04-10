#include "console.h"

int read_line(void)
{
	int size = BUF_SIZE;

	char *buf = malloc(size);
	if (buf == NULL) {
		perror("I/O error");
		return -1;
	}

	char c;
	while ((c = getchar()) != EOF && c != '\n') {
		if (ftell(stdin) >= size) {
			size <<= 1;
			buf = realloc(buf, size);
		}
	}

	return 0;

error:
	free(buf);
	perror("I/O error");
	return -1;
}
