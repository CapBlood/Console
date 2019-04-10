CC = @gcc-8
CFLAGS = -w -c
LFLAGS = -lcurses
NAME = console
INS_DIR = /Users/donsangre/Documents/Scripts

all: $(NAME)

clean:
	-@rm *.o
	-@rm $(NAME)

install: $(NAME)
	@if [ -d $(INS_DIR) ]; \
	then \
		cp $(NAME) $(INS_DIR) && \
		echo 'Successfully installed :)'; \
	else \
		echo 'Sorry, i can''t install this app :('; \
	fi

pack: $(NAME)
	@tar -cf $(NAME).tar $(NAME)

del_debug:
	@strip $(NAME)

console.o: console.c console.h
main.o: main.c console.h

$(NAME): main.o console.o
	-$(CC) $(LFLAGS) main.o console.o -o $(NAME)
