SRCS=jsh.c

CC=gcc
CFLAGS=-Wall -std=gnu99

all:
	$(CC) $(CFLAGS) $(SRCS) -o bin/jsh

clean:
	rm -f bin/*
