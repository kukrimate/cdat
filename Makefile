##
# Makefile to build libkm tests
##

# Everything must work under strict C99 with all warning
CFLAGS := -Isrc \
		  -std=c99 -Wall -Wextra -Wpedantic \
		  -D_GNU_SOURCE -O1 -g

# Test binaries to build
TESTS := test_vec test_map test_set test_pq

# Build all tests by default
.PHONY: all
all: $(TESTS)

test_%: test/%.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Run all tests
.PHONY: test
test: $(TESTS)
	for TEST in $(TESTS); do \
		valgrind ./$$TEST; \
	done

.PHONY: clean
clean:
	rm -f $(TESTS)
