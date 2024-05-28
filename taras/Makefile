CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 

all: s21_grep

s21_grep:
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep
	
test:
	bash grep_test.sh

clean:
	rm -rf s21_grep 
