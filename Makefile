CFLAGS=-Wall -Wextra

all: test read_elf

test: test.c
	$(CC) $(CFLAGS) $^ -o $@

read_elf: read_elf.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f test
