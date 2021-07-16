CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS =

testrealloc : assoc.h specific.h realloc.c testassoc.c general.h general.c
	$(CC) testassoc.c realloc.c general.c -o testrealloc $(PRODUCTION) $(LDLIBS)

testrealloc_s : assoc.h specific.h realloc.c testassoc.c general.h general.c
	$(CC) testassoc.c realloc.c general.c -o testrealloc_s $(SANITIZE) $(LDLIBS)

testrealloc_v : assoc.h specific.h realloc.c testassoc.c general.h general.c
	$(CC) testassoc.c realloc.c general.c -o testrealloc_v $(VALGRIND) $(LDLIBS)

testcuckoo_s : assoc.h specific.h cuckoo.c testassoc.c general.h general.c
	$(CC) testassoc.c cuckoo.c general.c -o testcuckoo_s $(SANITIZE) $(LDLIBS)

testcuckoo_v : assoc.h Cuckoo/specific.h Cuckoo/cuckoo.c testassoc.c ../../ADTs/General/general.h ../../ADTs/General/general.c
	$(CC) testassoc.c Cuckoo/cuckoo.c ../../ADTs/General/general.c -o testcuckoo_v -I./Cuckoo $(VALGRIND) $(LDLIBS)

testcuckoo : assoc.h Cuckoo/specific.h Cuckoo/cuckoo.c testassoc.c ../../ADTs/General/general.h ../../ADTs/General/general.c
	$(CC) testassoc.c Cuckoo/cuckoo.c ../../ADTs/General/general.c -o testcuckoo -I./Cuckoo $(PRODUCTION) $(LDLIBS)

clean:
	rm -f testrealloc_s testrealloc_v testrealloc testcuckoo_s testcuckoo_v testcuckoo

basic: testrealloc_s testrealloc_v
	./testrealloc_s
	valgrind ./testrealloc_v

cuckoo: testcuckoo_s testcuckoo_v
	./testcuckoo_s
	valgrind ./testcuckoo_v
