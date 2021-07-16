cc = gcc
#cc = clang
#CFLAGS = -Wall -Wextra -pedantic -std=c90 -O3
CFLAGS = -Wall -Wextra -pedantic -std=c90 -g3 -fsanitize=undefined -fsanitize=address
LDLIBS = -lm

b1 : main_b1.c func_b1.c common.h
	$(CC) main_b1.c func_b1.c -o b1 $(CFLAGS) $(LDLIBS)
