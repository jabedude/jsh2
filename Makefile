SRCS=jsh.c

CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=gnu99

all:
	$(CC) $(CFLAGS) $(SRCS) -o bin/jsh

clean:
	rm -f bin/*
