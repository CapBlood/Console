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

	if (init_buffer() == -1)
		goto error;


	login = getlogin();
	while (1) {
		printf("%s$ -> ", login);
		// read_key();
		buf = read_string();
		put_command(buf);
		args = split(buf);
		execute(args);
		free(buf);
		free(args);
	}

	// while ((str = get_command()) != NULL) {
	// 	printf("%s\n", str);
	// }



	del_buffer();
	return 0;

error:
	perror("SuperTerm");
	return -1;
}
