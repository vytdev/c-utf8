# C compiler to use
CC = clang
# flags to pass to the compiler and linker
CFLAGS = -std=c99 -Wall -pedantic
LDFLAGS =

all: utf8

utf8.o: utf8.c utf8.h
	$(CC) $(CFLAGS) -c $< -o $@

test.o: test.c
	$(CC) $(CFLAGS) -c $< -o $@

utf8: utf8.o test.o
	$(CC) $(LDFLAGS) $^ -o $@

# our sub-commands
.PHONY: debug clean

# debug mode
debug: CFLAGS += -D__DEBUG -g
debug: LDFLAGS += -g
debug: all

# clean the worktree
clean:
	rm -rf test.o utf8.o utf8
