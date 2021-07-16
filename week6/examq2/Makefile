CC = gcc
#CC = clang
COMMON = -Wall -Wextra -pedantic -std=c90 -Wfloat-equal
SANITIZE =  -g3 -fsanitize=undefined -fsanitize=address
OPT = -O3
LDLIBS = -lm

b1_s : main_b1.c func_b1.c common.h
	$(CC) main_b1.c func_b1.c common.h -o b1_s $(COMMON) $(SANITIZE) $(LDLIBS)

b1_o : main_b1.c func_b1.c common.h
	$(CC) main_b1.c func_b1.c common.h -o b1_o $(COMMON) $(OPT) $(LDLIBS)
