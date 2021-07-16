CC = gcc
#CC = clang
COMMON = -Wall -Wextra -pedantic -std=c90 -Wfloat-equal
SANITIZE =  -g3 -fsanitize=undefined -fsanitize=address
OPT = -O3
LDLIBS = -lm

out_s : prime.c primefuncs.c prime.h
	$(CC) prime.c primefuncs.c -o out_s $(COMMON) $(SANITIZE) $(LDLIBS)

out_o : prime.c primefuncs.c prime.h
	$(CC) prime.c primefuncs.c -o out_o $(COMMON) $(SANITIZE) $(LDLIBS)
