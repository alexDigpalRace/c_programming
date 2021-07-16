#CC = gcc
CC = clang
COMMON = -Wall -Wextra -pedantic -std=c90 -Wfloat-equal
SANITIZE =  -g3 -fsanitize=undefined -fsanitize=address
OPT = -O3
LDLIBS = -lm

bookcase : bookcase.c
	$(CC) bookcase.c -o bookcase $(COMMON) $(SANITIZE) $(LDLIBS)

bookcase_o : bookcase.c
	$(CC) bookcase.c -o bookcase_o $(COMMON) $(SANITIZE) $(LDLIBS)
