CC = gcc
#CC = clang
COMMON = -Wall -Wextra -pedantic -std=c90 -Wfloat-equal
SANITIZE =  -g3 -fsanitize=undefined -fsanitize=address
OPT = -O3
LDLIBS = -lm

spellcheck_s : spellcheck.c spellcheck.h
	$(CC) spellcheck.c -o spellcheck_s $(COMMON) $(SANITIZE) $(LDLIBS)

spellcheck_o : spellcheck.c spellcheck.h
	$(CC) spellcheck.c -o spellcheck_o $(COMMON) $(SANITIZE) $(LDLIBS)
