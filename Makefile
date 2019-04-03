GCOV_OUTPUT = *.gcda *.gcno *.gcov 
GCOV_CCFLAGS = -fprofile-arcs -ftest-coverage
CC     = gcc
CCFLAGS = -I. -Itests -g -O2 -Wall -Werror -W -fno-omit-frame-pointer -fno-common -fsigned-char $(GCOV_CCFLAGS)


all: test

main.c:
	sh tests/make-tests.sh tests/test_*.c > main.c

test: main.c bipbuffer.o tests/test_bipbuffer.c tests/CuTest.c main.c
	$(CC) $(CCFLAGS) -o $@ $^
	./test
	gcov main.c tests/test_heap.c heap.c

bipbuffer.o: bipbuffer.c
	$(CC) $(CCFLAGS) -c -o $@ $^

clean:
	rm -f main.c bipbuffer.o test $(GCOV_OUTPUT)
