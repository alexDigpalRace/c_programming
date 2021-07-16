CC= gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 #-Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
SDL= `sdl2-config --cflags`
PRODUCTION= $(COMMON) -O3
LDLIBS = `sdl2-config --libs` -lm


parse : parser/turtle.c parser/turtle.h Makefile
	$(CC) parser/turtle.c -o parse $(SANITIZE) $(LDLIBS)

interp : interpreter/turtle.c interpreter/turtle.h interpreter/adaptedsdl2.c interpreter/adaptedsdl2.h Makefile
	$(CC) interpreter/turtle.c interpreter/adaptedsdl2.c -o interp $(SANITIZE) $(SDL) $(LDLIBS)

extension : ext/turtle.c ext/turtle.h ext/adaptedsdl2.c ext/adaptedsdl2.h Makefile
	$(CC) ext/turtle.c ext/adaptedsdl2.c -o extension $(SANITIZE) $(SDL) $(LDLIBS)
