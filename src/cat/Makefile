CC = gcc -Wall -Wextra -Werror

all: cat test

cat: main.c cat_functions.c
	$(CC) main.c cat_functions.c -o cat

test:
	test_cat/test_cat.sh

clean:
	rm cat

rebuild:
	make clean
	make all
