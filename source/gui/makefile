CC = g++
MOC = $(QTDIR)/bin/moc
CFLAGS = -std=c++11 -I$(QTDIR)/lib/QtWidgets.framework/Headers -I$(QTDIR)/lib/QtGui.framework/Headers -F$(QTDIR)/lib
LFLAGS = -Wl,-rpath,$(QTDIR)/lib -framework QtWidgets -framework QtCore -framework QtGui

all: test

lib.moc: lib.h
	$(MOC) lib.h > lib.moc

lib.o: lib.cpp lib.moc
	$(CC) -c $(CFLAGS) lib.cpp -o lib.o

test: lib.o
	$(CC) $(CFLAGS) $(LFLAGS) qt_test.cpp lib.o -o test
