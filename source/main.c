#include "console.h"

/*
█████████████████████████████
█───█────█─██─█────█────█───█
█─███─██─█──█─█─████─██─█─███
█───█────█─█──█─█──█────█───█
███─█─██─█─██─█─██─█─█─██─███
█───█─██─█─██─█────█─█─██───█
█████████████████████████████
*/

int main(void)
{
	char *login;
	char **args;
	char *buf;

	login = getlogin();
	while (1) {
		printf("%s$ -> ", login);
		buf = read_string();
		args = split(buf);
		execute(args);
		free(buf);
		free(args);
	}

	return 0;

error:
	perror("SuperTerm");
	return -1;
}
