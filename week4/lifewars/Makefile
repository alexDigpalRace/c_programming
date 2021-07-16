cc = gcc
#CFLAGS = -Wall -Wextra -pedantic -std=c90 -O3
CFLAGS = -Wall -Wextra -pedantic -std=c90 -g3 -fsanitize=undefined -fsanitize=address
LDLIBS = -lm

lifewars : lifewars.c lifewarsfunc.c lifewarsfunc.h
	$(CC) lifewars.c lifewarsfunc.c -o lifewars $(CFLAGS) $(LDLIBS)
