CC = gcc -Wall -Wextra -Werror
TARGET = grep

all: grep test

grep: main.c grep_functions.c
	$(CC) main.c grep_functions.c -o grep

test:
	test_grep/test_grep.sh

clean:
	rm $(TARGET)

rebuild:
	make clean
	make all
