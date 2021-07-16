cc = gcc
#cc = clang
#CFLAGS = -Wall -Wextra -pedantic -std=c90 -O3
CFLAGS = -Wall -Wextra -pedantic -std=c90 -g3 -fsanitize=undefined -fsanitize=address
LDLIBS = -lm

a2 : main_a2.c func_a2.c common.h
	$(CC) main_a2.c func_a2.c -o a2 $(CFLAGS) $(LDLIBS)
