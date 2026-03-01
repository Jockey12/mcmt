CC = gcc
CFLAGS = -O2
LIBS = -lncurses -lmenu

all: mcmt

mcmt: src/main.c src/libs.h
	$(CC) $(CFLAGS) -o mcmt src/main.c $(LIBS)
