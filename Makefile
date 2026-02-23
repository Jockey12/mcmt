CC = gcc
CFLAGS = -Wall -O2
LIBS = -lncurses

all: mcmt

mcmt: src/main.c src/libs.h
	$(CC) $(CFLAGS) -o mcmt src/main.c $(LIBS)
