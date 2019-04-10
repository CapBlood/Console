#include "console.h"

int read_line(void)
{
	int size = BUF_SIZE;
	char *buf = malloc(size);
	if (buf == NULL) {
		perror("I/O error");
		return -1;
	}

	int pos = 0;
	char c;
	while ((c = getchar()) != EOF && c != '\n') {
		if (pos >= size) {
			char *tmp = realloc(buf, size << 1);
			if (tmp == NULL)
				goto error;
			size <<= 1;
			buf = tmp;
		}
		buf[pos++] = c;
	}

	buf[pos] = 0;
	return 0;

error:
	free(buf);
	perror("I/O error");
	return -1;
}
