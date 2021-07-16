cc = gcc
common = -Wall -Wextra -Wfloat-equal -pedantic -std=c90
sanitize = -g3 -fsanitize=address -fsanitize=undefined
libs = -lm

depth : depth.c
	$(cc) depth.c -o depth $(common) $(sanitize)
