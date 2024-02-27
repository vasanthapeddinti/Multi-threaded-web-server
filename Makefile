CC=gcc
CFLAGS = -std=c11 -Wall -Wextra -Wwrite-strings -Wno-parentheses -Wpedantic -Warray-bounds -Wconversion -I.

all:
	$(CC) $(CFLAGS) server.c -o server
