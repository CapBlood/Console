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
	if (init_buffer() == -1)
		return -1;

	char *str;
	int state;
	do {
		str = read_string();
		state = strcmp(str, "exit\n");
		put_command(str);
		free(str);
	} while (state != 0);

	while ((str = get_command()) != NULL) {
		printf("%s\n", str);
	}

	// execute(args);

	del_buffer();
	return 0;
}
