SRCS=jsh.c

CC=gcc
CFLAGS=-Wall -std=gnu99

all:
	$(CC) $(CFLAGS) $(SRCS) -o jsh

clean:
	rm -rf jsh
