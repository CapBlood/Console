CC = @gcc-8
C++ = g++
CFLAGS = -w -c -g -fsanitize=address
LFLAGS = -g -fsanitize=address
NAME = console
NAME_QT = console_gui
DIR = ./source
MOC = $(QTDIR)/bin/moc
QCFLAGS = -w -std=c++11 -I$(QTDIR)/lib/QtWidgets.framework/Headers -I$(QTDIR)/lib/QtGui.framework/Headers -I$(QTDIR)/lib/QtCore.framework/Headers -F$(QTDIR)/lib
QLFLAGS = -Wl,-rpath,$(QTDIR)/lib -framework QtWidgets -framework QtCore -framework QtGui

all: $(NAME)

gui: $(NAME_QT)

clean:
	-@rm *.o
	-@rm $(NAME)

pack: $(NAME)
	@tar -cf $(NAME).tar $(NAME)

del_debug:
	@strip $(NAME)

console.o: $(DIR)/console.c $(DIR)/console.h
	$(CC) $(CFLAGS) $(DIR)/console.c -o $@

main.o: $(DIR)/main.c $(DIR)/console.h
	$(CC) $(CFLAGS) $(DIR)/main.c -o $@

$(NAME): main.o console.o
	$(CC) $(LFLAGS) main.o console.o -o $(NAME)

console++.o: $(DIR)/console.c $(DIR)/console.h
	$(C++) -c $(QCFLAGS) $(DIR)/console.c  -o $@

meta_lib.o: $(DIR)/lib.h
		$(MOC) $(DIR)/lib.h -o $(DIR)/moc_lib.cpp
		$(C++) -c $(QCFLAGS) -o meta_lib.o $(DIR)/moc_lib.cpp

lib.o: $(DIR)/lib.cpp
		$(C++) -c $(QCFLAGS) -o lib.o $(DIR)/lib.cpp

$(NAME_QT): lib.o console++.o meta_lib.o
		$(C++) $(QCFLAGS) $(QLFLAGS) $(DIR)/qt_test.cpp lib.o -o $(NAME_QT) meta_lib.o console++.o
