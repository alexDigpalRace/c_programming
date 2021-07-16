CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 #-Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = -lm

realloc : assoc.h specific.h realloc.c testassoc.c general.h general.c
	$(CC) testassoc.c realloc.c general.c -o realloc $(SANITIZE) $(LDLIBS)

realloc_o : assoc.h specific.h realloc.c testassoc.c general.h general.c
	$(CC) testassoc.c realloc.c general.c -o realloc_o $(PRODUCTION) $(LDLIBS)

testrealloc : assoc.h specific.h realloc.c testrealloc.c general.h general.c
	$(CC) testrealloc.c realloc.c general.c -o testrealloc $(SANITIZE) $(LDLIBS)
