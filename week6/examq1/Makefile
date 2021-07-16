CC = gcc
#CC = clang
COMMON = -Wall -Wextra -pedantic -std=c90 -Wfloat-equal
SANITIZE =  -g3 -fsanitize=undefined -fsanitize=address
OPT = -O3
LDLIBS = -lm

a3_s : main_a3.c func_a3.c common.h
	$(CC) main_a3.c func_a3.c common.h -o a3_s $(COMMON) $(SANITIZE) $(LDLIBS)

a3_o : main_a3.c func_a3.c common.h
	$(CC) main_a3.c func_a3.c common.h -o a3_o $(COMMON) $(OPT) $(LDLIBS)
