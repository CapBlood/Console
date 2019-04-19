#include "console.h"
#include <termios.h>

/*
█████████████████████████████
█───█────█─██─█────█────█───█
█─███─██─█──█─█─████─██─█─███
█───█────█─█──█─█──█────█───█
███─█─██─█─██─█─██─█─█─██─███
█───█─██─█─██─█────█─█─██───█
█████████████████████████████
*/

FILE *stream_in;
FILE *stream_out;

int main(void)
{
	// struct termios term_saved;
	// if (tcgetattr(STDIN_FILENO, &term_saved) == -1)
	// 	goto error;
	//
	// struct termios curr_term = term_saved;
	// curr_term.c_lflag &= ~ICANON;
	// // curr_term.c_lflag |= ECHO;
	//
	// curr_term.c_cc[VERASE] = "\0";
	// tcsetattr(STDIN_FILENO, TCSANOW, &curr_term);
	// tcsetattr(STDOUT_FILENO, TCSANOW, &curr_term);

	if (init_buffer() == -1)
		goto error;

	char *str;
	int state;
	do {
		str = read_string();
		state = strcmp(str, "exit");
		put_command(str);
		str = split(str);
		// printf("%s\n", str[0]);
		free(str);
	} while (state != 0);

	// while ((str = get_command()) != NULL) {
	// 	printf("%s\n", str);
	// }

	// execute(args);

	del_buffer();
	return 0;

error:
	perror("SuperTerm");
	return -1;
}
