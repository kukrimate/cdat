##
# Makefile to build libkm tests
##

CFLAGS := -ansi -D_GNU_SOURCE -g

all: test_dynarr test_htab

test_dynarr: dynarr.o test_dynarr.o
	$(CC) $(LDFLAGS) -o $@ $^

test_htab: htab.o test_htab.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

test: test_dynarr test_htab
	valgrind ./test_dynarr
	valgrind ./test_htab

clean:
	rm -f *.o test_dynarr test_htab
