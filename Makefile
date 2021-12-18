##
# Makefile to build libkm tests
##

CFLAGS = -Isrc -std=c99 -Wall -Wextra -Wpedantic -D_GNU_SOURCE -O1 -g

# Build test binary
TEST_OBJ = src/test/main.o

test: $(TEST_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -f test $(TEST_OBJ)
