CC = gcc
COMMON = -Wall -Wextra -Wfloat-equal -pedantic -std=c90
SANITIZE = -g3 -fsanitize=undefined -fsanitize=address
OPT = -O3
LDLIBS = -lm

realloc : specific.h realloc.c testboolarr.c
	$(CC) realloc.c general.c testboolarr.c -o realloc $(COMMON) $(SANITIZE) $(LDLIBS)
