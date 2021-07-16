cc = gcc
#CFLAGS = -Wall -Wextra -pedantic -std=c90 -O3
CFLAGS = -Wall -Wextra -pedantic -std=c90 -g3 -fsanitize=undefined -fsanitize=address
LDLIBS = -lm

anagrams : anagrams.c anagrams.h
	$(CC) anagrams.c -o anagrams $(CFLAGS) $(LDLIBS)

selfanagram : selfanagram.c anagrams.h
	$(CC) selfanagram.c -o selfanagram $(CFLAGS) $(LDLIBS)
