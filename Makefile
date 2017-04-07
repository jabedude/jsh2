SRCS=jsh.c

CC=gcc
CFLAGS=-Wall

all:
	$(CC) $(CFLAGS) $(SRCS) -o jsh

clean:
	rm -rf jsh
