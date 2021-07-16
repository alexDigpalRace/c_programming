CC = gcc
#CC = clang
COMMON = -Wall -Wextra -pedantic -std=c90 -Wfloat-equal
SANITIZE =  -g3 -fsanitize=undefined -fsanitize=address
OPT = -O3
LDLIBS = -lm

a1_s : main_a1.c func_a1.c common.h
	$(CC) main_a1.c func_a1.c common.h -o a1_s $(COMMON) $(SANITIZE) $(LDLIBS)

a1_o : main_a1.c func_a1.c common.h
	$(CC) main_a1.c func_a1.c common.h -o a1_o $(COMMON) $(OPT) $(LDLIBS)
